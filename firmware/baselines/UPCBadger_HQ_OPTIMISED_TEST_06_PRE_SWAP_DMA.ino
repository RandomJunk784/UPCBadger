/*
  UPCBadger HQ OPTIMISED TEST 06 - PRE-SWAP DMA

  GOAL:
    Start combining the proven pieces into the product-style player:

      full 16-bit RGB565
      separate SD SPI bus
      LovyanGFX DMA
      double buffering / read-ahead
      maximum practical SPI clock for the first test
      black startup
      animation plays once
      final frame remains displayed

  DISPLAY — DO NOT CHANGE:
    D23 = MOSI
    D18 = SCLK
    D21 = CS
    D22 = DC
    D4  = RST
    BL  = 3.3V

  SD — NEW SEPARATE BUS:
    D27 = MOSI
    D25 = SCLK
    D26 = MISO
    D13 = CS

  ASSET:
    /badger.raw
    360x360 RGB565
    Source frame rate retained
    Source frames retained
    ~4.04 seconds of animation
    Final frame is then held forever

  FIRST SPEED TARGET:
    TFT = 80 MHz
    SD  = 40 MHz

  WHY:
    LovyanGFX supports SPI transmit up to 80 MHz and DMA.
    The ESP32 Arduino SPI API supports a second SPIClass on HSPI
    with custom pin routing.

  IMPORTANT:
    This test intentionally does NOT allocate a full frame.
    Two 32 KB DMA buffers are used instead.
*/

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <SPI.h>
#include <SD.h>

// ============================================================
// DISPLAY PINS — LOCKED
// ============================================================

#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   21
#define TFT_DC   22
#define TFT_RST   4

// ============================================================
// SD PINS — SEPARATE BUS
// ============================================================

#define SD_MOSI 27
#define SD_SCLK 25
#define SD_MISO 26
#define SD_CS   13

// ============================================================
// VIDEO
// ============================================================

#define WIDTH  360
#define HEIGHT 360

#define FRAME_PIXELS (WIDTH * HEIGHT)
#define FRAME_BYTES  (FRAME_PIXELS * 2)

// Source is 97 frames at 24 FPS (~4.04 s).
#define FRAME_COUNT 97
#define SOURCE_FPS 24.0f

// ============================================================
// SPI SPEEDS
// ============================================================

#define TFT_SPI_HZ 80000000
#define SD_SPI_HZ  40000000

// ============================================================
// BUFFERING
//
// 16384 pixels = 32768 bytes.
// Two buffers = 64 KB total.
// ============================================================

#define CHUNK_PIXELS 16384
#define CHUNK_BYTES  (CHUNK_PIXELS * 2)

// ============================================================
// LOVYANGFX GC9B72
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

    pc.panel_width  = WIDTH;
    pc.panel_height = HEIGHT;
    pc.memory_width = WIDTH;
    pc.memory_height = HEIGHT;

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

// Separate HSPI bus for SD.
SPIClass sdSPI(HSPI);

// ============================================================
// DOUBLE BUFFER
// ============================================================

uint16_t bufferA[CHUNK_PIXELS];
uint16_t bufferB[CHUNK_PIXELS];

// ============================================================
// SD READ
// ============================================================

bool readChunk(
  File &file,
  uint16_t *buffer,
  size_t pixels
)
{
  size_t bytes = pixels * 2;

  size_t got = file.read(
    (uint8_t *)buffer,
    bytes
  );

  if (got != bytes)
  {
    Serial.print("SD short read: ");
    Serial.print(got);
    Serial.print("/");
    Serial.println(bytes);
    return false;
  }

  return true;
}

// ============================================================
// PRE-SWAP RGB565 FOR DIRECT DMA
// ============================================================
//
// /badger.raw is stored little-endian.
// TEST 05 used writePixelsDMA(..., true), which makes LovyanGFX
// perform the byte swap/conversion as part of the display write.
//
// Here we do the SAME byte transformation directly in our existing
// buffer, then use writePixelsDMA(..., false).
//
// This leaves the on-disk asset untouched and preserves the exact
// display byte order of the known-good TEST 05 image path.
//
// Two RGB565 pixels are swapped at once using a 32-bit operation.
//

static inline void preSwapRGB565(
  uint16_t *buffer,
  size_t pixels
)
{
  uint32_t *p32 = reinterpret_cast<uint32_t *>(buffer);
  size_t words = pixels / 2;

  for (size_t i = 0; i < words; ++i)
  {
    uint32_t x = p32[i];

    p32[i] =
      ((x & 0x00FF00FFUL) << 8) |
      ((x & 0xFF00FF00UL) >> 8);
  }

  if (pixels & 1)
  {
    uint16_t v = buffer[pixels - 1];
    buffer[pixels - 1] = (uint16_t)((v << 8) | (v >> 8));
  }
}

// ============================================================
// PLAY ONE FRAME
//
// A is displayed while B is loaded from SD.
// Then B is displayed while A is loaded.
// ============================================================

bool playFrame(File &file)
{
  size_t remaining = FRAME_PIXELS;

  // Preload first chunk.
  size_t currentPixels = min(
    (size_t)CHUNK_PIXELS,
    remaining
  );

  if (!readChunk(
        file,
        bufferA,
        currentPixels
      ))
  {
    return false;
  }

  preSwapRGB565(bufferA, currentPixels);

  remaining -= currentPixels;

  uint16_t *current = bufferA;
  uint16_t *next    = bufferB;

  tft.startWrite();

  // One complete 360x360 window.
  tft.setAddrWindow(
    0,
    0,
    WIDTH,
    HEIGHT
  );

  while (true)
  {
    // Start DMA for current buffer.
    // Buffer has already been byte-swapped in place.
    // swap=false sends it directly to the DMA engine.
    tft.writePixelsDMA(
      current,
      currentPixels,
      false
    );

    // While the display DMA is transferring CURRENT,
    // the independent HSPI bus loads NEXT.
    size_t nextPixels = 0;

    if (remaining > 0)
    {
      nextPixels = min(
        (size_t)CHUNK_PIXELS,
        remaining
      );

      if (!readChunk(
            file,
            next,
            nextPixels
          ))
      {
        tft.waitDMA();
        tft.endWrite();
        return false;
      }

      preSwapRGB565(next, nextPixels);

      remaining -= nextPixels;
    }

    // Do not overwrite a buffer while DMA is using it.
    tft.waitDMA();

    if (nextPixels == 0)
      break;

    uint16_t *tmp = current;
    current = next;
    next = tmp;
    currentPixels = nextPixels;
  }

  tft.endWrite();

  return true;
}

// ============================================================
// SETUP
// ============================================================

void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("==========================================");
  Serial.println(" UPCBadger HQ OPTIMISED TEST 06 - PRE-SWAP DMA");
  Serial.println(" FULL RGB565 + SEPARATE SPI + DMA");
  Serial.println("==========================================");

  Serial.println("DISPLAY:");
  Serial.println(" MOSI=23 SCLK=18 CS=21 DC=22 RST=4");

  Serial.println("SD:");
  Serial.println(" MOSI=27 SCLK=25 MISO=26 CS=13");

  Serial.print("TFT SPI: ");
  Serial.print(TFT_SPI_HZ / 1000000);
  Serial.println(" MHz");

  Serial.print("SD SPI:  ");
  Serial.print(SD_SPI_HZ / 1000000);
  Serial.println(" MHz");

  Serial.println("DMA: ON");
  Serial.println("RGB565: PRE-SWAP IN READ BUFFER -> DIRECT DMA");
  Serial.println("Buffers: 2 x 32 KB");
  Serial.println("Startup: BLACK");
  Serial.println("Behaviour: PLAY ONCE -> HOLD FINAL FRAME");

  // ----------------------------------------------------------
  // TFT
  // ----------------------------------------------------------

  Serial.println();
  Serial.println("Starting GC9B72...");

  if (!tft.init())
  {
    Serial.println("ERROR: GC9B72 init failed");
    while (true)
      delay(1000);
  }

  tft.setRotation(0);
  tft.setColorDepth(16);
  tft.setSwapBytes(false);
  tft.initDMA();

  // Product-style startup: black.
  tft.fillScreen(TFT_BLACK);

  Serial.println("GC9B72 OK");
  Serial.println("DMA OK");

  // ----------------------------------------------------------
  // SD
  // ----------------------------------------------------------

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

  // ----------------------------------------------------------
  // OPEN ASSET
  // ----------------------------------------------------------

  File file = SD.open(
    "/badger.raw",
    FILE_READ
  );

  if (!file)
  {
    Serial.println("ERROR: /badger.raw not found");
    while (true)
      delay(1000);
  }

  const uint32_t expectedBytes =
    (uint32_t)FRAME_BYTES * FRAME_COUNT;

  Serial.print("Raw file size: ");
  Serial.println((uint32_t)file.size());

  Serial.print("Expected: ");
  Serial.println(expectedBytes);

  if ((uint32_t)file.size() != expectedBytes)
  {
    Serial.println("ERROR: wrong raw file size");
    file.close();
    while (true)
      delay(1000);
  }

  // ----------------------------------------------------------
  // PLAYBACK
  // ----------------------------------------------------------

  Serial.println();
  Serial.println("PLAYBACK START");

  uint64_t totalProcessingMs = 0;

  for (int frame = 0; frame < FRAME_COUNT; frame++)
  {
    uint32_t start = millis();

    if (!playFrame(file))
    {
      Serial.print("ERROR: FRAME ");
      Serial.print(frame);
      Serial.println(" FAILED");
      file.close();

      while (true)
        delay(1000);
    }

    uint32_t elapsed = millis() - start;

    totalProcessingMs += elapsed;

    // No fixed delay.
    // Run at the maximum speed the hardware actually achieves.
    //
    // We deliberately do not fake 24 FPS with delays.
    // This is a speed/quality benchmark.

    if ((frame + 1) % 12 == 0)
    {
      Serial.print("Frame ");
      Serial.print(frame + 1);
      Serial.print("/");
      Serial.print(FRAME_COUNT);
      Serial.print(" | ");
      Serial.print(elapsed);
      Serial.println(" ms");
    }
  }

  file.close();

  // ----------------------------------------------------------
  // FINAL FRAME HOLD
  //
  // The final frame is already on the TFT.
  // Hold it for the remainder of a nominal 5-second boot.
  // Then keep holding it indefinitely.
  // ----------------------------------------------------------

  const uint32_t nominalAnimationMs =
    (uint32_t)((FRAME_COUNT / SOURCE_FPS) * 1000.0f);

  uint32_t finalHoldMs = 5000;

  if (nominalAnimationMs < 5000)
    finalHoldMs = 5000 - nominalAnimationMs;
  else
    finalHoldMs = 0;

  Serial.println();
  Serial.println("ANIMATION COMPLETE");

  Serial.print("Animation processing average: ");
  Serial.print(
    (double)totalProcessingMs / FRAME_COUNT,
    2
  );
  Serial.println(" ms/frame");

  Serial.print("Processing-only FPS: ");
  Serial.println(
    1000.0 /
    ((double)totalProcessingMs / FRAME_COUNT),
    2
  );

  Serial.print("Nominal source duration: ");
  Serial.print(nominalAnimationMs);
  Serial.println(" ms");

  Serial.print("Final-frame startup hold: ");
  Serial.print(finalHoldMs);
  Serial.println(" ms");

  if (finalHoldMs > 0)
    delay(finalHoldMs);

  Serial.println("FINAL FRAME HOLDING");
}

void loop()
{
  // Finished product behaviour:
  // final logo/frame remains displayed until power is removed.
  delay(1000);
}