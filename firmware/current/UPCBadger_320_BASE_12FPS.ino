struct StageReader;

/*
  UPCBadger TEST 12C
  IMPORTANT: KEEP THIS AS THE ONLY .ino FILE IN THE SKETCH FOLDER.
  Arduino concatenates every .ino in a sketch folder; do not place
  TEST 7D/7C/etc. beside this file.
*/
/*
  UPCBadger TEST 12
  320 SAFE-CIRCULAR CBV + TRUE DMA PIPELINE

  GOAL:
    Push the corrected 320-pixel artwork at the maximum practical
    rate while preserving the 360x360 TFT canvas.

  VIDEO:
    /ConsoleBadger_320_CIRCULAR_SAFE_DIRECTDMA.CBV
    107 frames @ 24 FPS
    360x360 output canvas
    320x320 sharp artwork footprint
    circular edge feather to black
    RGB565 bytes stored in DIRECT-DMA byte order

  CBV:
    independent 32 KiB LZ4 blocks
    96 KiB compressed staging buffer
    two 32 KiB decoded DMA buffers

  PIPELINE:
      TFT DMA buffer A  -> TFT
                   while DMA runs:
      SD/stage -> decode buffer B
                   then swap
      TFT DMA buffer B  -> TFT
                   while DMA runs:
      SD/stage -> decode buffer A
                   ...

  This intentionally removes the runtime RGB565 byte-swap.

  IMPORTANT:
    This is a TEST build. It does NOT replace any known-good baseline.

  HARDWARE:
    TFT MOSI 23
    TFT SCLK 18
    TFT CS   21
    TFT DC   22
    TFT RST   4
    TFT SPI 80 MHz

    SD MOSI 27
    SD SCLK 25
    SD MISO 26
    SD CS   13
    SD request 40 MHz

  SAFETY:
    UNPLUG THE ESP32 BEFORE CHANGING WIRING.
*/

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <SPI.h>
#include <SD.h>
#include "esp_heap_caps.h"

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
// DISPLAY
// ============================================================

#define WIDTH 320
#define HEIGHT 320

#define FRAME_PIXELS (WIDTH * HEIGHT)
#define FRAME_BYTES  (FRAME_PIXELS * 2)

#define TFT_SPI_HZ 80000000UL
#define SD_SPI_HZ  40000000UL

// ============================================================
// CBV
// ============================================================

#define CBV_FILE "/ConsoleBadger_320_CIRCULAR_SAFE_CROPPED_DIRECTDMA_HC9.CBV"

#define FRAME_COUNT_EXPECTED 107
#define BLOCK_BYTES 32768

// This is intentionally below the largest ~110 KiB contiguous
// heap block observed after TFT initialisation.
#define STAGING_BYTES (96 * 1024)

#define DIRECT_DMA_FLAG 0x0002

// ============================================================
// MEMORY
// ============================================================

// Static buffers avoid heap fragmentation.
// They are used one at a time by the TFT DMA engine.
static uint8_t dmaBufferA[BLOCK_BYTES];
static uint8_t dmaBufferB[BLOCK_BYTES];

// Compressed staging buffer.
// It is normal RAM, not DMA RAM.
static uint8_t *stageBuffer = nullptr;

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

    cfg.spi_host = VSPI_HOST;
    cfg.spi_mode = 0;
    cfg.freq_write = TFT_SPI_HZ;
    cfg.freq_read = 16000000;

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

    pc.panel_width  = 360;
    pc.panel_height = 360;
    pc.memory_width = 360;
    pc.memory_height = 360;

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
// CBV STRUCTURES
// ============================================================

#pragma pack(push, 1)

struct CBVHeader
{
  char magic[4];
  uint16_t version;
  uint16_t codec;
  uint16_t width;
  uint16_t height;
  uint32_t fpsNum;
  uint32_t fpsDen;
  uint32_t frameCount;
  uint32_t frameBytes;
  uint32_t blockBytes;
  uint16_t indexEntryBytes;
  uint16_t flags;
  uint32_t indexOffset;
  uint32_t dataOffset;
};

struct CBVIndexEntry
{
  uint32_t offset;
  uint32_t storedBytes;
  uint16_t blockCount;
  uint16_t flags;
  uint32_t crc32;
};

struct CBVBlockHeader
{
  uint32_t storedBytes;
  uint16_t rawBytes;
  uint8_t flags;
  uint8_t reserved;
};

#pragma pack(pop)

static CBVHeader cbvHeader;
static CBVIndexEntry indexTable[FRAME_COUNT_EXPECTED];

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

  if (r.endPos == STAGING_BYTES)
  {
    if (!stageCompact(r))
      return false;
  }

  size_t freeSpace =
    STAGING_BYTES - r.endPos;

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
  if (wanted > STAGING_BYTES)
    return false;

  while (
    (r.endPos - r.beginPos) < wanted
  )
  {
    if (r.fileRemaining == 0)
      return false;

    if (r.endPos == STAGING_BYTES)
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
// LZ4
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

    // --------------------------------------------------------
    // Literals
    // --------------------------------------------------------

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

    if (literalLength)
    {
      memcpy(
        dst + di,
        src + si,
        literalLength
      );

      si += literalLength;
      di += literalLength;
    }

    // A final literal run is legal and has no match.
    if (si >= srcSize)
      break;

    // --------------------------------------------------------
    // Match offset
    // --------------------------------------------------------

    if (si + 2 > srcSize)
      return false;

    uint16_t offset =
      (uint16_t)src[si] |
      ((uint16_t)src[si + 1] << 8);

    si += 2;

    if (offset == 0 || offset > di)
      return false;

    // --------------------------------------------------------
    // Match length
    // --------------------------------------------------------

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

    uint8_t *matchDst = dst + di;
    uint8_t *matchSrc = dst + di - offset;

    // --------------------------------------------------------
    // FAST MATCH COPY
    //
    // LZ4 allows overlapping matches. The important optimisation
    // is to expand the repeated pattern in chunks rather than
    // copying one byte at a time.
    //
    // Case A:
    //   offset >= matchLength
    //   Source/destination do not overlap -> memcpy.
    //
    // Case B:
    //   offset < matchLength
    //   Copy one 'offset'-byte seed repeatedly. Each destination
    //   chunk is immediately available as the source for the next
    //   chunk, preserving LZ4's overlap semantics.
    // --------------------------------------------------------

    if (offset >= matchLength)
    {
      memcpy(
        matchDst,
        matchSrc,
        matchLength
      );
    }
    else
    {
      size_t remaining = matchLength;

      // First chunk is the existing match source.
      size_t chunk = offset;

      if (chunk > remaining)
        chunk = remaining;

      memcpy(
        matchDst,
        matchSrc,
        chunk
      );

      remaining -= chunk;
      di += chunk;

      // Subsequent chunks can copy from the already-expanded
      // destination. Source and destination ranges are adjacent,
      // so memcpy is safe for each chunk.
      while (remaining)
      {
        size_t copyNow = offset;

        if (copyNow > remaining)
          copyNow = remaining;

        memcpy(
          dst + di,
          dst + di - offset,
          copyNow
        );

        di += copyNow;
        remaining -= copyNow;
      }

      continue;
    }

    di += matchLength;
  }

  written = di;

  return written == expectedOutput;
}

// ============================================================
// CBV HEADER / INDEX
// ============================================================

static bool loadCBVIndex(File &file)
{
  file.seek(0);

  if (file.read(
        (uint8_t *)&cbvHeader,
        sizeof(cbvHeader)
      ) != sizeof(cbvHeader))
  {
    Serial.println("ERROR: CBV header read failed");
    return false;
  }

  if (memcmp(cbvHeader.magic, "CBV1", 4) != 0)
  {
    Serial.println("ERROR: CBV magic mismatch");
    return false;
  }

  if (cbvHeader.version != 1 ||
      cbvHeader.codec != 1)
  {
    Serial.println("ERROR: unsupported CBV");
    return false;
  }

  if (cbvHeader.width != WIDTH ||
      cbvHeader.height != HEIGHT ||
      cbvHeader.frameBytes != FRAME_BYTES)
  {
    Serial.println("ERROR: geometry mismatch");
    return false;
  }

  if (cbvHeader.frameCount != FRAME_COUNT_EXPECTED)
  {
    Serial.println("ERROR: frame count mismatch");
    return false;
  }

  if (cbvHeader.blockBytes != BLOCK_BYTES)
  {
    Serial.println("ERROR: block size mismatch");
    return false;
  }

  if (!(cbvHeader.flags & DIRECT_DMA_FLAG))
  {
    Serial.println("ERROR: CBV is not marked DIRECT-DMA");
    return false;
  }

  if (cbvHeader.indexEntryBytes != sizeof(CBVIndexEntry))
  {
    Serial.println("ERROR: index entry size mismatch");
    return false;
  }

  file.seek(cbvHeader.indexOffset);

  size_t indexBytes =
    cbvHeader.frameCount *
    sizeof(CBVIndexEntry);

  if (file.read(
        (uint8_t *)indexTable,
        indexBytes
      ) != indexBytes)
  {
    Serial.println("ERROR: index read failed");
    return false;
  }

  uint64_t sum = 0;
  uint32_t maxPayload = 0;

  for (uint32_t i = 0;
       i < cbvHeader.frameCount;
       ++i)
  {
    sum += indexTable[i].storedBytes;

    if (indexTable[i].storedBytes > maxPayload)
      maxPayload = indexTable[i].storedBytes;
  }

  Serial.print("CBV average frame payload: ");
  Serial.println(
    (double)sum / cbvHeader.frameCount,
    1
  );

  Serial.print("CBV max frame payload: ");
  Serial.println(maxPayload);

  return true;
}

// ============================================================
// PREPARE NEXT BLOCK
// ============================================================

static bool prepareNextBlock(
  StageReader &reader,
  uint8_t *destination,
  size_t &written,
  uint32_t &decodeUs
)
{
  if (!stageEnsure(
        reader,
        sizeof(CBVBlockHeader)
      ))
  {
    return false;
  }

  CBVBlockHeader bh;

  memcpy(
    &bh,
    stageBuffer + reader.beginPos,
    sizeof(bh)
  );

  if (bh.rawBytes == 0 ||
      bh.rawBytes > BLOCK_BYTES ||
      bh.storedBytes == 0 ||
      bh.storedBytes > BLOCK_BYTES)
  {
    Serial.println("ERROR: invalid CBV block");
    return false;
  }

  size_t fullBytes =
    sizeof(CBVBlockHeader) +
    (size_t)bh.storedBytes;

  if (!stageEnsure(
        reader,
        fullBytes
      ))
  {
    return false;
  }

  const uint8_t *src =
    stageBuffer +
    reader.beginPos +
    sizeof(CBVBlockHeader);

  uint32_t startDecode = micros();

  if (bh.flags & 0x01)
  {
    if (bh.storedBytes != bh.rawBytes)
      return false;

    memcpy(
      destination,
      src,
      bh.rawBytes
    );

    written = bh.rawBytes;
  }
  else
  {
    if (!lz4DecompressBlock(
          src,
          bh.storedBytes,
          destination,
          BLOCK_BYTES,
          bh.rawBytes,
          written
        ))
    {
      return false;
    }
  }

  decodeUs +=
    micros() - startDecode;

  reader.beginPos += fullBytes;

  if (reader.beginPos == reader.endPos)
  {
    reader.beginPos = 0;
    reader.endPos = 0;
  }
  else if (reader.beginPos >= STAGING_BYTES / 2)
  {
    stageCompact(reader);
  }

  // Keep enough compressed data queued where possible.
  const size_t lowWater =
    sizeof(CBVBlockHeader) + BLOCK_BYTES;

  if (
    (reader.endPos - reader.beginPos) < lowWater &&
    reader.fileRemaining > 0
  )
  {
    if (!stageFill(reader))
      return false;
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
  const CBVIndexEntry &entry =
    indexTable[frameIndex];

  StageReader reader;

  reader.file = &file;
  reader.beginPos = 0;
  reader.endPos = 0;
  reader.fileRemaining = entry.storedBytes;

  uint8_t *current = dmaBufferA;
  uint8_t *next = dmaBufferB;

  size_t currentBytes = 0;
  size_t nextBytes = 0;

  // Start timing BEFORE the initial SD read so the complete frame
  // time includes the pipeline's first fill.
  uint32_t frameStart = micros();

  // Read and decode the first block before starting DMA.
  if (!prepareNextBlock(
        reader,
        current,
        currentBytes,
        decodeUs
      ))
    return false;

  tft.startWrite();
  tft.setAddrWindow(20, 20, WIDTH, HEIGHT);

  uint32_t pixelsSent = 0;
  const uint16_t blockCount =
    indexTable[frameIndex].blockCount;

  if (blockCount == 0)
  {
    tft.endWrite();
    return false;
  }

  for (uint16_t block = 0; block < blockCount; ++block)
  {
    // Start DMA for the current decoded block.
    tft.writePixelsDMA(
      reinterpret_cast<uint16_t *>(current),
      currentBytes / 2,
      false
    );

    bool haveNext = false;

    // While the TFT DMA is running, prepare the next block.
    if (block + 1 < blockCount)
    {
      haveNext =
        prepareNextBlock(
          reader,
          next,
          nextBytes,
          decodeUs
        );

      if (!haveNext)
      {
        tft.waitDMA();
        tft.endWrite();
        return false;
      }
    }

    uint32_t waitStart = micros();

    tft.waitDMA();

    dmaUs +=
      micros() - waitStart;

    pixelsSent +=
      currentBytes / 2;

    if (!haveNext)
      break;

    uint8_t *tmp = current;
    current = next;
    next = tmp;

    currentBytes = nextBytes;
    nextBytes = 0;
  }

  tft.endWrite();

  frameUs =
    micros() - frameStart;

  if (reader.fileRemaining != 0)
  {
    Serial.println("ERROR: CBV frame read incomplete");
    return false;
  }

  if (reader.beginPos != reader.endPos)
  {
    Serial.println("ERROR: CBV frame payload not exhausted");
    return false;
  }

  if (pixelsSent != FRAME_PIXELS)
  {
    Serial.print("ERROR: pixels sent ");
    Serial.print(pixelsSent);
    Serial.print(" expected ");
    Serial.println(FRAME_PIXELS);
    return false;
  }

  sdUs = (uint32_t)reader.sdUs;
  stageUs = (uint32_t)reader.stageUs;

  return true;
}

// ============================================================
// SETUP
// ============================================================

void setup()
{
  Serial.begin(115200);
  delay(1200);

  Serial.println();
  Serial.println();
  Serial.println("==========================================");
  Serial.println(" UPCBadger TEST 12");
  Serial.println(" 320 WINDOW + HC9 + OPTIMISED LZ4 + DMA PIPELINE");
  Serial.println("==========================================");

  Serial.println("TFT: 360x360 / 80 MHz");
  Serial.println("Panel: 360x360");
  Serial.println("Transfer window: 320x320 at (20,20)");
  Serial.println("Artwork: 320x320 / circular feather");
  Serial.println("CBV: HC9 DIRECT-DMA RGB565");
  Serial.println("Pipeline: SD/stage/decode || TFT DMA");
  Serial.println("Buffers: 2 x 32 KiB DMA");
  Serial.println("Staging: 96 KiB");

  // Stage buffer first.
  Serial.println();
  Serial.println("Allocating staging buffer...");

  stageBuffer =
    (uint8_t *)heap_caps_malloc(
      STAGING_BYTES,
      MALLOC_CAP_8BIT
    );

  if (!stageBuffer)
  {
    Serial.println("ERROR: staging allocation failed");

    Serial.print("Largest free block: ");
    Serial.println(
      heap_caps_get_largest_free_block(
        MALLOC_CAP_8BIT
      )
    );

    while (true)
      delay(1000);
  }

  Serial.println("Staging buffer OK.");

  // TFT.
  Serial.println();
  Serial.println("Starting GC9B72...");

  if (!tft.init())
  {
    Serial.println("ERROR: TFT init failed");

    while (true)
      delay(1000);
  }

  tft.setRotation(0);
  tft.setColorDepth(16);
  tft.setSwapBytes(false);
  tft.initDMA();
  tft.fillScreen(TFT_BLACK);

  Serial.println("GC9B72 OK");
  Serial.println("DMA OK");

  // SD.
  Serial.println();
  Serial.println("Starting SD on HSPI...");

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
    Serial.println("ERROR: SD CARD FAILED");

    while (true)
      delay(1000);
  }

  Serial.println("SD CARD OK");

  File file =
    SD.open(
      CBV_FILE,
      FILE_READ
    );

  if (!file)
  {
    Serial.print("ERROR: ");
    Serial.print(CBV_FILE);
    Serial.println(" NOT FOUND");

    while (true)
      delay(1000);
  }

  Serial.print("CBV file size: ");
  Serial.println((uint32_t)file.size());

  if (!loadCBVIndex(file))
  {
    file.close();

    while (true)
      delay(1000);
  }

  Serial.print("Data starts at byte: ");
  Serial.println(cbvHeader.dataOffset);

  // ----------------------------------------------------------
  // Playback.
  // ----------------------------------------------------------

  Serial.println();
  Serial.println("==========================================");
  Serial.println("CBV PIPELINED PLAYBACK START");
  Serial.println("==========================================");

  uint64_t totalFrameUs = 0;
  uint64_t totalSdUs = 0;
  uint64_t totalDecodeUs = 0;
  uint64_t totalDmaUs = 0;
  uint64_t totalStageUs = 0;

  uint32_t maxFrameUs = 0;

  for (uint32_t frame = 0;
       frame < cbvHeader.frameCount;
       ++frame)
  {
    uint32_t frameUs = 0;
    uint32_t sdUs = 0;
    uint32_t decodeUs = 0;
    uint32_t dmaUs = 0;
    uint32_t stageUs = 0;

    if (!playFramePipelined(
          file,
          frame,
          frameUs,
          sdUs,
          decodeUs,
          dmaUs,
          stageUs
        ))
    {
      Serial.print("ERROR: FRAME ");
      Serial.print(frame + 1);
      Serial.println(" FAILED");

      file.close();

      while (true)
        delay(1000);
    }

    totalFrameUs += frameUs;
    totalSdUs += sdUs;
    totalDecodeUs += decodeUs;
    totalDmaUs += dmaUs;
    totalStageUs += stageUs;

    if (frameUs > maxFrameUs)
      maxFrameUs = frameUs;

    if ((frame + 1) % 12 == 0 ||
        frame + 1 == cbvHeader.frameCount)
    {
      Serial.print("Frame ");
      Serial.print(frame + 1);
      Serial.print("/");
      Serial.print(cbvHeader.frameCount);

      Serial.print(" | total ");
      Serial.print(frameUs / 1000.0, 2);

      Serial.print(" ms | SD ");
      Serial.print(sdUs / 1000.0, 2);

      Serial.print(" ms | decode ");
      Serial.print(decodeUs / 1000.0, 2);

      Serial.print(" ms | DMAwait ");
      Serial.print(dmaUs / 1000.0, 2);

      Serial.print(" ms | stage ");
      Serial.print(stageUs / 1000.0, 2);

      Serial.println(" ms");
    }
  }

  file.close();

  double avgFrameMs =
    ((double)totalFrameUs /
     cbvHeader.frameCount) / 1000.0;

  double avgSdMs =
    ((double)totalSdUs /
     cbvHeader.frameCount) / 1000.0;

  double avgDecodeMs =
    ((double)totalDecodeUs /
     cbvHeader.frameCount) / 1000.0;

  double avgDmaMs =
    ((double)totalDmaUs /
     cbvHeader.frameCount) / 1000.0;

  double avgStageMs =
    ((double)totalStageUs /
     cbvHeader.frameCount) / 1000.0;

  Serial.println();
  Serial.println("==========================================");
  Serial.println("TEST 8 COMPLETE");
  Serial.println("==========================================");

  Serial.print("Average frame:  ");
  Serial.print(avgFrameMs, 3);
  Serial.println(" ms");

  Serial.print("Average SD:     ");
  Serial.print(avgSdMs, 3);
  Serial.println(" ms");

  Serial.print("Average decode: ");
  Serial.print(avgDecodeMs, 3);
  Serial.println(" ms");

  Serial.print("Average DMAwait: ");
  Serial.print(avgDmaMs, 3);
  Serial.println(" ms");

  Serial.print("Average stage:  ");
  Serial.print(avgStageMs, 3);
  Serial.println(" ms");

  Serial.print("Slowest frame:  ");
  Serial.print(maxFrameUs / 1000.0, 3);
  Serial.println(" ms");

  Serial.print("Unpaced FPS:    ");
  Serial.println(
    1000.0 / avgFrameMs,
    3
  );

  Serial.println();
  Serial.println("DECODER:");
  Serial.println("Optimised LZ4 overlap copy enabled.");
  Serial.println("Runtime RGB565 swap remains OFF.");

  Serial.println();
  Serial.println("REFERENCE:");
  Serial.println("TEST 7C 320 CBV: 7.125 FPS");
  Serial.println("TEST 12 target: >12 FPS");

  Serial.println();
  Serial.println("FINAL FRAME HOLDING");
}

void loop()
{
  delay(1000);
}