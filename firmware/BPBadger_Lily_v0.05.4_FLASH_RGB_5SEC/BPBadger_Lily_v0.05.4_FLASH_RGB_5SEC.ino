/*
  BPBadger Lily S3 v0.05.4 FLASH RGB

  Standalone backpack badge for Lily.

  Hardware:
    ESP32-S3 N16R8
    2.1-inch round TFT
    LIS3DH accelerometer
    Internal LittleFS flash storage

  Behaviour:
    - Play Lily's animation repeatedly while awake.
    - Movement resets the 60-second inactivity timer.
    - 60 seconds without movement enters deep sleep.
    - LIS3DH INT1 wakes the ESP32-S3.

  Reused from the proven ConsoleBadger player:
    - LovyanGFX TFT setup
    - CBP1 palette/LZ4 decoder
    - staged frame reader
    - two-buffer DMA playback

  Removed:
    - Wi-Fi
    - XBL/API/HTTPS
    - web server/configuration portal
    - gamerpic/profile code
    - ConsoleBadger product layer
*/

// v0.05.2: based directly on the known-good v0.04 FLASH build.
// Added onboard ESP32-S3 addressable RGB LED movement indicator on GPIO48.
// Full-power pink/purple flashing is driven by the existing LIS3DH movement events.
// TFT, LittleFS, CBP decoder, LIS3DH pins/configuration and 60 s timer retained.

#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <LittleFS.h>
#include <Wire.h>
#include <LovyanGFX.hpp>
#include <esp_sleep.h>

#define TFT_MOSI 17
#define TFT_SCLK 18
#define TFT_CS   21
#define TFT_DC   16
#define TFT_RST   4


#define PANEL_WIDTH  360
#define PANEL_HEIGHT 360
#define WIDTH  360
#define HEIGHT 360
#define WINDOW_X 0
#define WINDOW_Y 0

#define FRAME_PIXELS (WIDTH * HEIGHT)
#define DISPLAY_FRAME_BYTES (FRAME_PIXELS * 2)

#define TFT_SPI_HZ 80000000UL

#define CBP_FILE "/Lily_Boot_Animation_12FPS.CBP"
#define CBP_VERSION 1
#define CBP_CODEC_PALETTE_LZ4 2
#define FRAME_COUNT_EXPECTED 122
#define BLOCK_INDEX_BYTES 16384
#define DIRECT_DMA_FLAG 0x0002
#define PALETTE_FLAG 0x0004
#define CBP_HEADER_BYTES 64
#define CBP_INDEX_ENTRY_BYTES 16
#define CBP_DATA_OFFSET (CBP_HEADER_BYTES + FRAME_COUNT_EXPECTED * CBP_INDEX_ENTRY_BYTES)
#define PALETTE_ENTRIES 256
#define PALETTE_BYTES (PALETTE_ENTRIES * 2)

static uint8_t dmaBufferA[32768];
static uint8_t dmaBufferB[32768];
#define STAGE_BUFFER_BYTES (32 * 1024)
static uint8_t *stageBuffer = nullptr;
static uint16_t palette565[PALETTE_ENTRIES];

#define LIS3DH_SDA  9
#define LIS3DH_SCL  8
#define LIS3DH_INT1 7
#define LIS3DH_ADDR_LOW  0x18
#define LIS3DH_ADDR_HIGH 0x19
#define LILY_ACTIVE_MS 60000UL
#define LILY_SHAKE_THRESHOLD 0x10

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_GC9B72 panel;
  lgfx::Bus_SPI bus;

public:
  LGFX()
  {
    auto cfg = bus.config();

    cfg.spi_host  = SPI3_HOST;
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
    pc.rgb_order = true;
    pc.dlen_16bit = false;
    pc.bus_shared = false;

    panel.config(pc);
    setPanel(&panel);
  }
};

LGFX tft;

// LIS3DH I2C address selected during startup (0x18 or 0x19).
static uint8_t lisAddr = 0;

// Active/sleep state and movement watchdog.
static uint32_t lastMovementMs = 0;
static bool lilyActive = true;

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

  
  uint32_t displayFrameBytes;

  
  uint32_t codedFrameBytes;

  
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


static_assert(
  STAGE_BUFFER_BYTES >= (sizeof(CBPBlockHeader) + BLOCK_INDEX_BYTES),
  "STAGE_BUFFER_BYTES too small for one CBP block"
);

static CBPHeader cbpHeader;
static CBPIndexEntry indexTable[FRAME_COUNT_EXPECTED];


struct StageReader
{
  File *file = nullptr;
  size_t beginPos = 0;
  size_t endPos = 0;
  uint32_t fileRemaining = 0;
  uint64_t storageUs = 0;
  uint64_t stageUs = 0;
};


// Onboard addressable RGB LED on the ESP32-S3 DevKit-style board.
#define RGB_LED_PIN 48
#define RGB_STEP_MS 5000UL

static uint32_t rgbLastStepMs = 0;
static bool rgbPurplePhase = false;

static void rgbOff()
{
  rgbLedWrite(RGB_LED_PIN, 0, 0, 0);
}

static void rgbShowPhase()
{
  if (rgbPurplePhase)
  {
    // Full-power purple.
    rgbLedWrite(RGB_LED_PIN, 180, 0, 255);
  }
  else
  {
    // Full-power pink.
    rgbLedWrite(RGB_LED_PIN, 255, 0, 180);
  }
}

static void rgbStartActivePeriod()
{
  // RGB follows the badge active/sleep state.
  rgbPurplePhase = false;
  rgbLastStepMs = millis();
  rgbShowPhase();
}

static void rgbService()
{
  const uint32_t now = millis();

  if (!lilyActive)
  {
    rgbOff();
    return;
  }

  if ((uint32_t)(now - rgbLastStepMs) >= RGB_STEP_MS)
  {
    rgbLastStepMs = now;
    rgbPurplePhase = !rgbPurplePhase;
    rgbShowPhase();
  }
}




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

  if (r.endPos == STAGE_BUFFER_BYTES)
  {
    if (!stageCompact(r))
      return false;
  }

  size_t freeSpace =
    STAGE_BUFFER_BYTES - r.endPos;

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

  r.storageUs +=
    micros() - start;

  if (got != toRead)
  {
    Serial.print("[CRASH] stage read ");
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
  if (wanted > STAGE_BUFFER_BYTES)
    return false;

  while (
    (r.endPos - r.beginPos) < wanted
  )
  {
    if (r.fileRemaining == 0)
      return false;

    if (r.endPos == STAGE_BUFFER_BYTES)
    {
      if (!stageCompact(r))
        return false;
    }

    if (!stageFill(r))
      return false;
  }

  return true;
}


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


static bool loadCBPIndex(File &file)
{
  file.seek(0);

  if (file.read(
        (uint8_t *)&cbpHeader,
        sizeof(cbpHeader)
      ) != sizeof(cbpHeader))
  {
    Serial.println("[CRASH] CBP header read failed");
    return false;
  }

  if (memcmp(cbpHeader.magic, "CBP1", 4) != 0)
  {
    Serial.println("[CRASH] CBP magic mismatch");
    return false;
  }

  if (cbpHeader.version != CBP_VERSION ||
      cbpHeader.codec != CBP_CODEC_PALETTE_LZ4)
  {
    Serial.println("[CRASH] unsupported CBP codec/version");
    return false;
  }

  if (cbpHeader.width != WIDTH ||
      cbpHeader.height != HEIGHT)
  {
    Serial.println("[CRASH] CBP geometry mismatch");
    return false;
  }

  if (cbpHeader.frameCount != FRAME_COUNT_EXPECTED)
  {
    Serial.println("[CRASH] CBP frame count mismatch");
    return false;
  }

  if (cbpHeader.displayFrameBytes !=
      DISPLAY_FRAME_BYTES)
  {
    Serial.println("[CRASH] CBP display bytes mismatch");
    return false;
  }

  if (cbpHeader.codedFrameBytes !=
      FRAME_PIXELS)
  {
    Serial.println("[CRASH] CBP coded bytes mismatch");
    return false;
  }

  if (cbpHeader.blockIndexBytes !=
      BLOCK_INDEX_BYTES)
  {
    Serial.println("[CRASH] CBP block size mismatch");
    return false;
  }

  if (!(cbpHeader.flags & DIRECT_DMA_FLAG))
  {
    Serial.println("[CRASH] CBP is not DIRECT-DMA");
    return false;
  }

  if (!(cbpHeader.flags & PALETTE_FLAG))
  {
    Serial.println("[CRASH] CBP is not palette encoded");
    return false;
  }

  if (cbpHeader.indexEntryBytes !=
      sizeof(CBPIndexEntry))
  {
    Serial.println("[CRASH] CBP index entry mismatch");
    return false;
  }

  if (cbpHeader.indexOffset != CBP_HEADER_BYTES ||
      cbpHeader.dataOffset != CBP_DATA_OFFSET)
  {
    Serial.println("[CRASH] CBP offsets mismatch");
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
    Serial.println("[CRASH] CBP index read failed");
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
  else if (reader.beginPos >= (STAGE_BUFFER_BYTES / 2))
  {
    stageCompact(reader);
  }

  return true;
}


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
    Serial.println("[CRASH] invalid CBP block");
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
  else if (reader.beginPos >= (STAGE_BUFFER_BYTES / 2))
  {
    stageCompact(reader);
  }

  return true;
}


static bool playFramePipelined(
  File &file,
  uint32_t frameIndex,
  uint32_t &frameUs,
  uint32_t &storageUs,
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

  
  if (!loadPalette(reader))
    return false;

  
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

  storageUs =
    (uint32_t)reader.storageUs;

  stageUs =
    (uint32_t)reader.stageUs;

  return true;
}
static bool initAnimationStorage()
{
  Serial.println("[INFO] Mounting LittleFS animation storage...");

  if (!LittleFS.begin(false))
  {
    Serial.println("[CRASH] LittleFS mount failed.");
    return false;
  }

  size_t total = LittleFS.totalBytes();
  size_t used  = LittleFS.usedBytes();

  Serial.print("[INFO] LittleFS: ");
  Serial.print(used);
  Serial.print("/");
  Serial.print(total);
  Serial.println(" bytes used");

  File file = LittleFS.open(CBP_FILE, FILE_READ);
  if (!file)
  {
    Serial.println("[CRASH] Lily CBP asset not found in LittleFS.");
    return false;
  }

  Serial.print("[INFO] Lily CBP size: ");
  Serial.print(file.size());
  Serial.println(" bytes");

  bool ok = loadCBPIndex(file);
  file.close();

  if (!ok)
  {
    Serial.println("[CRASH] Lily CBP validation failed.");
    return false;
  }

  Serial.print("[INFO] Lily CBP OK: ");
  Serial.print(cbpHeader.frameCount);
  Serial.print(" frames, ");
  Serial.print(cbpHeader.width);
  Serial.print("x");
  Serial.print(cbpHeader.height);
  Serial.print(" @ ");
  Serial.print(cbpHeader.fpsNum);
  Serial.print("/");
  Serial.print(cbpHeader.fpsDen);
  Serial.println(" FPS");

  return true;
}

static bool playLilyAnimation()
{
  File file = LittleFS.open(CBP_FILE, FILE_READ);

  if (!file)
  {
    Serial.println("[CRASH] Lily CBP open failed.");
    return false;
  }

  for (uint32_t frame = 0;
       frame < FRAME_COUNT_EXPECTED && frame < cbpHeader.frameCount;
       ++frame)
  {
    if (!file.seek(indexTable[frame].offset))
    {