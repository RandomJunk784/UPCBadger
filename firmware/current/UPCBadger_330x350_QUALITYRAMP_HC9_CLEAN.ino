/*
  UPCBadger 330x350 QUALITY-RAMP CLEAN
  96/192/256-COLOUR HC9 PLAYBACK
  256-COLOR PALETTE + LZ4 HC9 + DIRECT-DMA PIPELINE

  PURPOSE:
    Attack the remaining SD bottleneck without changing the
    physical 360x360 display or the current visual composition.

  CODEC:
    CBP1 = ConsoleBadger Palette Video, version 1
    256-color per-frame RGB565 palette
    8-bit palette indices
    independent 16 KiB LZ4 blocks
    palette + compressed indices are pre-built on the PC

  WHY:
    The current 330x350 RGB565 CBV carries 16 bits per pixel.
    This candidate carries 8 bits per pixel plus a 512-byte palette,
    then expands indices back to RGB565 immediately before DMA.

    The test asset was generated from the current visual master.
    Average RGB888 palette reconstruction error was measured at
    ~1.41 levels/channel on the cropped 330x350 image sequence.

  DISPLAY — LOCKED:
    MOSI 23
    SCLK 18
    CS   21
    DC   22
    RST   4
    TFT SPI = 80 MHz

  SD — LOCKED PINS:
    MOSI 27
    SCLK 25
    MISO 26
    CS   13

  IMPORTANT:
    Arduino ESP32 SD.h is retained.
    No custom File buffer is used.
    The previous 64 KiB File buffer test made performance much worse.

  MEMORY:
    96 KiB compressed staging
    2 x 32 KiB DMA buffers
    256-entry RGB565 palette = 512 bytes

  SAFETY:
    UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.
    Keep this as the ONLY .ino file in its Arduino sketch folder.
*/

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <SPI.h>
#include <SD.h>

// ============================================================
// PINS
// ============================================================

#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   21
#define TFT_DC   22
#define TFT_RST   4

#define SD_MOSI 27
#define SD_SCLK 25
#define SD_MISO 26
#define SD_CS   13

// ============================================================
// DISPLAY / ACTIVE WINDOW
// ============================================================

// Physical panel remains 360x360.
// Video intentionally uses the proven 330x350 window:
// X=15..344, Y=5..354.

#define PANEL_WIDTH  360
#define PANEL_HEIGHT 360

#define WIDTH  330
#define HEIGHT 350

#define WINDOW_X 15
#define WINDOW_Y 5

#define FRAME_PIXELS (WIDTH * HEIGHT)
#define DISPLAY_FRAME_BYTES (FRAME_PIXELS * 2)

// ============================================================
// SPI
// ============================================================

#define TFT_SPI_HZ 80000000UL
#define SD_SPI_HZ  40000000UL

// ============================================================
// CBP1
// ============================================================

#define CBP_FILE "/ConsoleBadger_330x350_HYBRID96_192_256_HC9_TEST27B.CBP"

#define CBP_VERSION 1
#define CBP_CODEC_PALETTE_LZ4 2

#define FRAME_COUNT_EXPECTED 107
#define BLOCK_INDEX_BYTES 16384

#define DIRECT_DMA_FLAG 0x0002
#define PALETTE_FLAG     0x0004

#define CBP_HEADER_BYTES 64
#define CBP_INDEX_ENTRY_BYTES 16
#define CBP_DATA_OFFSET \
  (CBP_HEADER_BYTES + FRAME_COUNT_EXPECTED * CBP_INDEX_ENTRY_BYTES)

// Palette = 256 RGB565 entries = 512 bytes.
#define PALETTE_ENTRIES 256
#define PALETTE_BYTES   (PALETTE_ENTRIES * 2)

// ============================================================
// MEMORY
// ============================================================

// Two 32 KiB decoded DMA buffers.
// A compressed 16 KiB index block expands to exactly 32 KiB RGB565.
static uint8_t dmaBufferA[32768];
static uint8_t dmaBufferB[32768];

// Compressed CBP data staging.
// Normal 8-bit-capable RAM, not a DMA buffer.
static uint8_t *stageBuffer = nullptr;

// Current per-frame palette.
// File stores byte-swapped RGB565 so a direct uint16 load produces
// the correct MSB-first bytes in little-endian ESP32 memory.
static uint16_t palette565[PALETTE_ENTRIES];

// ============================================================
// LOVYANGFX
// ============================================================

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_GC9B72 panel;
  lgfx::Bus_SPI bus;

public:
  LGFX()
  {
    auto cfg = bus.config();

    cfg.spi_host  = VSPI_HOST;
    cfg.spi_mode  = 0;
    cfg.freq_write = TFT_SPI_HZ;
    cfg.freq_read  = 16000000;

    cfg.pin_sclk = TFT_SCLK;
    cfg.pin_mosi = TFT_MOSI;
    cfg.pin_miso = -1;
    cfg.pin_dc   = TFT_DC;

    cfg.dma_channel = SPI_DMA_CH_AUTO;

    bus.config(cfg);
    panel.setBus(&bus);

    auto pc = panel.config();

    pc.pin_cs   = TFT_CS;
    pc.pin_rst  = TFT_RST;
    pc.pin_busy = -1;

    pc.panel_width  = PANEL_WIDTH;
    pc.panel_height = PANEL_HEIGHT;
    pc.memory_width = PANEL_WIDTH;
    pc.memory_height = PANEL_HEIGHT;

    pc.offset_x = 0;
    pc.offset_y = 0;
    pc.offset_rotation = 0;

    pc.readable = false;
    pc.invert = false;
    pc.rgb_order = false;
    pc.dlen_16bit = false;
    pc.bus_shared = false;

    panel.config(pc);
    setPanel(&panel);
  }
};

LGFX tft;
SPIClass sdSPI(HSPI);

// ============================================================
// CBP STRUCTURES
// ============================================================

#pragma pack(push, 1)

struct CBPHeader
{
  char magic[4];
  uint16_t version;
  uint16_t codec;
  uint16_t width;
  uint16_t height;

  uint32_t fpsNum;
  uint32_t fpsDen;
  uint32_t frameCount;

  // Decoded display bytes per frame.
  uint32_t displayFrameBytes;

  // Encoded palette-index bytes per frame.
  uint32_t codedFrameBytes;

  // Maximum raw index bytes represented by one LZ4 block.
  uint32_t blockIndexBytes;

  uint16_t indexEntryBytes;
  uint16_t flags;

  uint32_t indexOffset;
  uint32_t dataOffset;
};

struct CBPIndexEntry
{
  uint32_t offset;
  uint32_t storedBytes;
  uint16_t blockCount;
  uint16_t flags;
  uint32_t crc32;
};

struct CBPBlockHeader
{
  uint32_t storedBytes;
  uint16_t rawBytes;
  uint8_t flags;
  uint8_t reserved;
};

#pragma pack(pop)

static_assert(sizeof(CBPHeader) <= CBP_HEADER_BYTES,
              "CBP header larger than 64 bytes");

static_assert(sizeof(CBPIndexEntry) == CBP_INDEX_ENTRY_BYTES,
              "CBP index entry size mismatch");

static_assert(sizeof(CBPBlockHeader) == 8,
              "CBP block header size mismatch");

static CBPHeader cbpHeader;
static CBPIndexEntry indexTable[FRAME_COUNT_EXPECTED];

// ============================================================
// STAGING READER
// ============================================================

struct StageReader
{
  File *file = nullptr;

  size_t beginPos = 0;
  size_t endPos = 0;

  uint32_t fileRemaining = 0;

  uint64_t sdUs = 0;
  uint64_t stageUs = 0;
};

static bool stageCompact(StageReader &r)
{
  if (r.beginPos == 0)
    return true;

  size_t leftover =
    r.endPos - r.beginPos;

  if (leftover > 0)
  {
    uint32_t start = micros();

    memmove(
      stageBuffer,
      stageBuffer + r.beginPos,
      leftover
    );

    r.stageUs +=
      micros() - start;
  }

  r.beginPos = 0;
  r.endPos = leftover;

  return true;
}

static bool stageFill(StageReader &r)
{
  if (r.fileRemaining == 0)
    return true;

  if (r.endPos == (96 * 1024))
  {
    if (!stageCompact(r))
      return false;
  }

  size_t freeSpace =
    (96 * 1024) - r.endPos;

  size_t toRead =
    min(
      freeSpace,
      (size_t)r.fileRemaining
    );

  uint32_t start = micros();

  size_t got =
    r.file->read(
      stageBuffer + r.endPos,
      toRead
    );

  r.sdUs +=
    micros() - start;

  if (got != toRead)
  {
    Serial.print("ERROR: stage read ");
    Serial.print(got);
    Serial.print("/");
    Serial.println(toRead);
    return false;
  }

  r.endPos += got;
  r.fileRemaining -= got;

  return true;
}

static bool stageEnsure(
  StageReader &r,
  size_t wanted
)
{
  if (wanted > (96 * 1024))
    return false;

  while (
    (r.endPos - r.beginPos) < wanted
  )
  {
    if (r.fileRemaining == 0)
      return false;

    if (r.endPos == (96 * 1024))
    {
      if (!stageCompact(r))
        return false;
    }

    if (!stageFill(r))
      return false;
  }

  return true;
}

// ============================================================
// LZ4 BLOCK DECODER
// ============================================================

static bool lz4DecompressBlock(
  const uint8_t *src,
  size_t srcSize,
  uint8_t *dst,
  size_t dstCapacity,
  size_t expectedOutput,
  size_t &written
)
{
  size_t si = 0;
  size_t di = 0;

  while (si < srcSize)
  {
    uint8_t token = src[si++];

    size_t literalLength =
      token >> 4;

    if (literalLength == 15)
    {
      while (true)
      {
        if (si >= srcSize)
          return false;

        uint8_t extra = src[si++];
        literalLength += extra;

        if (extra != 255)
          break;
      }
    }

    if (si + literalLength > srcSize)
      return false;

    if (di + literalLength > dstCapacity)
      return false;

    if (literalLength > 0)
    {
      memcpy(
        dst + di,
        src + si,
        literalLength
      );

      si += literalLength;
      di += literalLength;
    }

    if (si >= srcSize)
      break;

    if (si + 2 > srcSize)
      return false;

    uint16_t offset =
      (uint16_t)src[si] |
      ((uint16_t)src[si + 1] << 8);

    si += 2;

    if (offset == 0 || offset > di)
      return false;

    size_t matchLength =
      (token & 0x0F) + 4;

    if ((token & 0x0F) == 15)
    {
      while (true)
      {
        if (si >= srcSize)
          return false;

        uint8_t extra = src[si++];
        matchLength += extra;

        if (extra != 255)
          break;
      }
    }

    if (di + matchLength > dstCapacity)
      return false;

    size_t matchPos =
      di - offset;

    for (size_t i = 0;
         i < matchLength;
         ++i)
    {
      dst[di++] =
        dst[matchPos + i];
    }
  }

  written = di;

  return written == expectedOutput;
}

// ============================================================
// CBP HEADER / INDEX
// ============================================================

static bool loadCBPIndex(File &file)
{
  file.seek(0);

  if (file.read(
        (uint8_t *)&cbpHeader,
        sizeof(cbpHeader)
      ) != sizeof(cbpHeader))
  {
    Serial.println("ERROR: CBP header read failed");
    return false;
  }

  if (memcmp(cbpHeader.magic, "CBP1", 4) != 0)
  {
    Serial.println("ERROR: CBP magic mismatch");
    return false;
  }

  if (cbpHeader.version != CBP_VERSION ||
      cbpHeader.codec != CBP_CODEC_PALETTE_LZ4)
  {
    Serial.println("ERROR: unsupported CBP codec/version");
    return false;
  }

  if (cbpHeader.width != WIDTH ||
      cbpHeader.height != HEIGHT)
  {
    Serial.println("ERROR: CBP geometry mismatch");
    return false;
  }

  if (cbpHeader.frameCount != FRAME_COUNT_EXPECTED)
  {
    Serial.println("ERROR: CBP frame count mismatch");
    return false;
  }

  if (cbpHeader.displayFrameBytes !=
      DISPLAY_FRAME_BYTES)
  {
    Serial.println("ERROR: CBP display bytes mismatch");
    return false;
  }

  if (cbpHeader.codedFrameBytes !=
      FRAME_PIXELS)
  {
    Serial.println("ERROR: CBP coded bytes mismatch");
    return false;
  }

  if (cbpHeader.blockIndexBytes !=
      BLOCK_INDEX_BYTES)
  {
    Serial.println("ERROR: CBP block size mismatch");
    return false;
  }

  if (!(cbpHeader.flags & DIRECT_DMA_FLAG))
  {
    Serial.println("ERROR: CBP is not DIRECT-DMA");
    return false;
  }

  if (!(cbpHeader.flags & PALETTE_FLAG))
  {
    Serial.println("ERROR: CBP is not palette encoded");
    return false;
  }

  if (cbpHeader.indexEntryBytes !=
      sizeof(CBPIndexEntry))
  {
    Serial.println("ERROR: CBP index entry mismatch");
    return false;
  }

  if (cbpHeader.indexOffset != CBP_HEADER_BYTES ||
      cbpHeader.dataOffset != CBP_DATA_OFFSET)
  {
    Serial.println("ERROR: CBP offsets mismatch");
    return false;
  }

  file.seek(cbpHeader.indexOffset);

  size_t indexBytes =
    cbpHeader.frameCount *
    sizeof(CBPIndexEntry);

  if (file.read(
        (uint8_t *)indexTable,
        indexBytes
      ) != indexBytes)
  {
    Serial.println("ERROR: CBP index read failed");
    return false;
  }

  uint64_t sum = 0;
  for (uint32_t i = 0;
       i < cbpHeader.frameCount;
       ++i)
  {
    sum +=
      indexTable[i].storedBytes;

  }

  return true;
}

// ============================================================
// LOAD FRAME PALETTE
// ============================================================

static bool loadPalette(StageReader &reader)
{
  if (!stageEnsure(reader, PALETTE_BYTES))
    return false;

  memcpy(
    palette565,
    stageBuffer + reader.beginPos,
    PALETTE_BYTES
  );

  reader.beginPos +=
    PALETTE_BYTES;

  if (reader.beginPos == reader.endPos)
  {
    reader.beginPos = 0;
    reader.endPos = 0;
  }
  else if (reader.beginPos >= (48 * 1024))
  {
    stageCompact(reader);
  }

  return true;
}

// ============================================================
// PREPARE NEXT CBP BLOCK
// ============================================================

static bool prepareNextBlock(
  StageReader &reader,
  uint8_t *destination,
  size_t &pixelCount,
  uint32_t &decodeUs
)
{
  if (!stageEnsure(
        reader,
        sizeof(CBPBlockHeader)
      ))
  {
    return false;
  }

  CBPBlockHeader bh;

  memcpy(
    &bh,
    stageBuffer + reader.beginPos,
    sizeof(bh)
  );

  if (bh.rawBytes == 0 ||
      bh.rawBytes > BLOCK_INDEX_BYTES ||
      bh.storedBytes == 0 ||
      bh.storedBytes > BLOCK_INDEX_BYTES)
  {
    Serial.println("ERROR: invalid CBP block");
    return false;
  }

  size_t fullBytes =
    sizeof(CBPBlockHeader) +
    (size_t)bh.storedBytes;

  if (!stageEnsure(reader, fullBytes))
    return false;

  const uint8_t *src =
    stageBuffer +
    reader.beginPos +
    sizeof(CBPBlockHeader);

  uint32_t startDecode = micros();

  size_t indexBytesWritten = 0;

  if (bh.flags & 0x01)
  {
    if (bh.storedBytes != bh.rawBytes)
      return false;

    memcpy(
      destination,
      src,
      bh.rawBytes
    );

    indexBytesWritten =
      bh.rawBytes;
  }
  else
  {
    if (!lz4DecompressBlock(
          src,
          bh.storedBytes,
          destination,
          16384,
          bh.rawBytes,
          indexBytesWritten
        ))
    {
      return false;
    }
  }

  // The index bytes are at the start of the same 32 KiB DMA buffer.
  // Expand backwards in-place to RGB565 so the source indices are
  // never overwritten before they are consumed.
  uint16_t *pixels =
    reinterpret_cast<uint16_t *>(destination);

  for (size_t i = indexBytesWritten;
       i > 0;
       --i)
  {
    size_t n = i - 1;

    uint8_t index =
      destination[n];

    pixels[n] =
      palette565[index];
  }

  pixelCount =
    indexBytesWritten;

  decodeUs +=
    micros() - startDecode;

  reader.beginPos += fullBytes;

  if (reader.beginPos == reader.endPos)
  {
    reader.beginPos = 0;
    reader.endPos = 0;
  }
  else if (reader.beginPos >= (48 * 1024))
  {
    stageCompact(reader);
  }

  return true;
}

// ============================================================
// PIPELINED FRAME
// ============================================================

static bool playFramePipelined(
  File &file,
  uint32_t frameIndex,
  uint32_t &frameUs,
  uint32_t &sdUs,
  uint32_t &decodeUs,
  uint32_t &dmaUs,
  uint32_t &stageUs
)
{
  const CBPIndexEntry &entry =
    indexTable[frameIndex];

  StageReader reader;

  reader.file = &file;
  reader.beginPos = 0;
  reader.endPos = 0;

  reader.fileRemaining =
    entry.storedBytes;

  uint8_t *current =
    dmaBufferA;

  uint8_t *next =
    dmaBufferB;

  size_t currentPixels = 0;
  size_t nextPixels = 0;

  uint32_t frameStart =
    micros();

  // Palette comes first in every frame.
  if (!loadPalette(reader))
    return false;

  // Decode the first block before starting DMA.
  if (!prepareNextBlock(
        reader,
        current,
        currentPixels,
        decodeUs
      ))
  {
    return false;
  }

  tft.startWrite();

  // Single 330x350 address window.
  // The 18-pixel side margins on the 360x360 panel are never sent.
  tft.setAddrWindow(
    WINDOW_X,
    WINDOW_Y,
    WIDTH,
    HEIGHT
  );

  uint32_t pixelsSent = 0;

  const uint16_t blockCount =
    entry.blockCount;

  if (blockCount == 0)
  {
    tft.endWrite();
    return false;
  }

  for (uint16_t block = 0;
       block < blockCount;
       ++block)
  {
    tft.writePixelsDMA(
      reinterpret_cast<uint16_t *>(current),
      currentPixels,
      false
    );

    bool haveNext = false;

    if (block + 1 < blockCount)
    {
      haveNext =
        prepareNextBlock(
          reader,
          next,
          nextPixels,
          decodeUs
        );

      if (!haveNext)
      {
        uint32_t waitStart =
          micros();

        tft.waitDMA();

        dmaUs +=
          micros() - waitStart;

        tft.endWrite();
        return false;
      }
    }

    uint32_t waitStart =
      micros();

    tft.waitDMA();

    dmaUs +=
      micros() - waitStart;

    pixelsSent +=
      currentPixels;

    if (!haveNext)
      break;

    uint8_t *tmp =
      current;

    current = next;
    next = tmp;

    currentPixels =
      nextPixels;

    nextPixels = 0;
  }

  tft.endWrite();

  frameUs =
    micros() - frameStart;

  if (reader.fileRemaining != 0)
  {
    Serial.println(
      "ERROR: CBP frame not fully consumed"
    );

    return false;
  }

  if (reader.beginPos != reader.endPos)
  {
    Serial.println(
      "ERROR: CBP stage data not exhausted"
    );

    return false;
  }

  if (pixelsSent != FRAME_PIXELS)
  {
    Serial.print(
      "ERROR: pixels sent "
    );
    Serial.print(pixelsSent);
    Serial.print(" expected ");
    Serial.println(FRAME_PIXELS);
    return false;
  }

  sdUs =
    (uint32_t)reader.sdUs;

  stageUs =
    (uint32_t)reader.stageUs;

  return true;
}


// ============================================================
// SETUP
// ============================================================

void setup()
{
  Serial.begin(115200);
  delay(300);

  // ----------------------------------------------------------
  // Allocate staging.
  // ----------------------------------------------------------
  stageBuffer =
    (uint8_t *)malloc(96 * 1024);

  if (!stageBuffer)
  {
    while (true)
      delay(1000);
  }

  // ----------------------------------------------------------
  // TFT.
  // ----------------------------------------------------------
  if (!tft.init())
  {
    while (true)
      delay(1000);
  }

  tft.setRotation(0);
  tft.setColorDepth(16);
  tft.setSwapBytes(false);
  tft.initDMA();
  tft.fillScreen(TFT_BLACK);

  // ----------------------------------------------------------
  // SD.
  // ----------------------------------------------------------
  sdSPI.begin(
    SD_SCLK,
    SD_MISO,
    SD_MOSI,
    SD_CS
  );

  if (!SD.begin(
        SD_CS,
        sdSPI,
        SD_SPI_HZ
      ))
  {
    while (true)
      delay(1000);
The requested file reference is not currently visible. Use files.search or files.list to rediscover the file, then retry with a returned ref_id or file_id.