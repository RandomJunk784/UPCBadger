/*
  UPCBadger SD PRODUCT FULL-MEMORY TEST V6 SD FIRST

  PURPOSE:
    V2 proved the product globals + 96 KiB staging buffer do not stop
    the known-good Arduino SD.h path.

    V3 adds the remaining static memory footprint used by the real
    CBP player:
      - 2 x 32 KiB DMA buffers
      - 256-entry RGB565 palette
      - 107-entry CBP index table

    NO CBP PLAYBACK.
    NO playFramePipelined().
    NO decoder changes.
    NO Wi-Fi AP.

  ORDER:
    TFT -> exact player memory footprint -> SD @ 40 MHz

  If SD passes:
    static memory footprint is not the cause and we move to the next
    behavioural difference in the full product startup.

  LOCKED HARDWARE:
    TFT: MOSI 23 / SCLK 18 / CS 21 / DC 22 / RST 4 / 80 MHz
    SD : MOSI 27 / SCLK 25 / MISO 26 / CS 13 / 40 MHz

  SAFETY:
    UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.
*/

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

// ------------------------------------------------------------
// LOCKED PINS / SPI
// ------------------------------------------------------------

#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   21
#define TFT_DC   22
#define TFT_RST   4

#define SD_MOSI 27
#define SD_SCLK 25
#define SD_MISO 26
#define SD_CS   13

#define TFT_SPI_HZ 80000000UL
#define SD_SPI_HZ  40000000UL

// ------------------------------------------------------------
// EXACT PLAYER-SIZE MEMORY
// ------------------------------------------------------------

#define FRAME_COUNT_EXPECTED 107
#define PALETTE_ENTRIES 256
#define CBP_INDEX_ENTRY_BYTES 16

static uint8_t dmaBufferA[32768];
static uint8_t dmaBufferB[32768];

static uint8_t *stageBuffer = nullptr;

static uint16_t palette565[PALETTE_ENTRIES];

struct CBPIndexEntry
{
  uint32_t offset;
  uint32_t storedBytes;
  uint16_t blockCount;
  uint16_t flags;
  uint32_t crc32;
};

static CBPIndexEntry indexTable[FRAME_COUNT_EXPECTED];

// Product-layer objects.
static WebServer setupServer(80);
static DNSServer setupDNS;
static Preferences setupPrefs;

static String savedSSID;
static String savedPassword;
static String savedGamerID;

// ------------------------------------------------------------
// TFT
// ------------------------------------------------------------

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_GC9B72 panel;
  lgfx::Bus_SPI bus;

public:
  LGFX()
  {
    auto cfg = bus.config();

    cfg.spi_host   = VSPI_HOST;
    cfg.spi_mode   = 0;
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

    pc.panel_width   = 360;
    pc.panel_height  = 360;
    pc.memory_width  = 360;
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

// ------------------------------------------------------------
// DIAGNOSTICS
// ------------------------------------------------------------

static void printHeap(const char *label)
{
  Serial.print("[MEM] ");
  Serial.print(label);
  Serial.print(" free=");
  Serial.print(ESP.getFreeHeap());
  Serial.print(" largest=");
  Serial.println(ESP.getMaxAllocHeap());
}

static void testNVS()
{
  Serial.println("[TEST] NVS");

  setupPrefs.begin("upcbadger", true);

  savedSSID = setupPrefs.getString("ssid", "");
  savedPassword = setupPrefs.getString("pass", "");
  savedGamerID = setupPrefs.getString("gt", "");

  setupPrefs.end();

  Serial.print("[NVS] SSID: ");
  Serial.println(savedSSID.length() ? "YES" : "NO");

  Serial.print("[NVS] Gamer ID: ");
  Serial.println(savedGamerID.length() ? "YES" : "NO");
}

// ------------------------------------------------------------
// SD TEST
// ------------------------------------------------------------

static bool testSD()
{
  Serial.println();
  Serial.println("------------------------------------------");
  Serial.println("[TEST] SD WITH FULL PLAYER MEMORY");
  Serial.println("------------------------------------------");

  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  printHeap("before HSPI");

  sdSPI.begin(
    SD_SCLK,
    SD_MISO,
    SD_MOSI,
    SD_CS
  );

  Serial.print("[SD] SD.begin @ ");
  Serial.print(SD_SPI_HZ / 1000000UL);
  Serial.println(" MHz");

  bool ok = SD.begin(
    SD_CS,
    sdSPI,
    SD_SPI_HZ
  );

  Serial.print("[SD] SD.begin result: ");
  Serial.println(ok ? "PASS" : "FAIL");

  if (!ok)
    return false;

  Serial.print("[SD] Card type: ");

  uint8_t type = SD.cardType();

  if (type == CARD_MMC)
    Serial.println("MMC");
  else if (type == CARD_SD)
    Serial.println("SDSC");
  else if (type == CARD_SDHC)
    Serial.println("SDHC");
  else if (type == CARD_NONE)
    Serial.println("NONE");
  else
    Serial.println("UNKNOWN");

  Serial.print("[SD] Card size MB: ");
  Serial.println(
    (uint32_t)(
      SD.cardSize() /
      (1024UL * 1024UL)
    )
  );

  File root = SD.open("/");

  Serial.print("[SD] Root open: ");
  Serial.println(root ? "PASS" : "FAIL");

  if (root)
    root.close();

  printHeap("after mount");

  return true;
}

// ------------------------------------------------------------
// SETUP
// ------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("==========================================");
  Serial.println(" UPCBadger SD PRODUCT FULL-MEMORY TEST V3");
  Serial.println(" PRODUCT GLOBALS + EXACT PLAYER RAM / SD BEFORE STAGING");
  Serial.println(" NO CBP PLAYBACK");
  Serial.println("==========================================");

  Serial.println();
  Serial.println("DISPLAY:");
  Serial.println(" MOSI=23 SCLK=18 CS=21 DC=22 RST=4");
  Serial.println(" TFT SPI: 80 MHz");

  Serial.println();
  Serial.println("SD:");
  Serial.println(" MOSI=27 SCLK=25 MISO=26 CS=13");
  Serial.println(" SD SPI: 40 MHz");

  Serial.println();
  Serial.println("[TFT] Starting...");

  if (!tft.init())
  {
    Serial.println("[TFT] INIT FAILED");
    while (true)
      delay(1000);
  }

  tft.setRotation(0);
  tft.setColorDepth(16);
  tft.setSwapBytes(false);
  tft.initDMA();
  tft.fillScreen(TFT_BLACK);

  Serial.println("[TFT] OK");

  Serial.println();
  Serial.println("[MEM] Delaying 96 KiB staging allocation until after SD mount.");

  // Do NOT write to the player buffers before SD.begin().
  // The known-good player does not touch them at startup.
  // Printing addresses keeps the objects materially referenced.
  Serial.print("[MEM] DMA A @ 0x");
  Serial.println((uintptr_t)dmaBufferA, HEX);

  Serial.print("[MEM] DMA B @ 0x");
  Serial.println((uintptr_t)dmaBufferB, HEX);

  Serial.print("[MEM] Palette @ 0x");
  Serial.println((uintptr_t)palette565, HEX);

  Serial.print("[MEM] Index @ 0x");
  Serial.println((uintptr_t)indexTable, HEX);

  Serial.println("[MEM] 2 x 32 KiB DMA buffers present");
  Serial.println("[MEM] 256-entry RGB565 palette present");
  Serial.println("[MEM] 107-entry CBP index present");

  printHeap("full static player memory, BEFORE staging");

  // The product layer exists, but does not touch NVS or Wi-Fi yet.
  if (!testSD())
  {
    Serial.println();
    Serial.println("[RESULT] SD FAIL WITH FULL PLAYER MEMORY");

    while (true)
      delay(1000);
  }

  Serial.println();
  Serial.println("[MEM] Allocating 96 KiB staging AFTER SD mount...");

  stageBuffer =
    (uint8_t *)malloc(96 * 1024);

  if (!stageBuffer)
  {
    Serial.println("[MEM] STAGING ALLOC FAILED AFTER SD MOUNT");

    while (true)
      delay(1000);
  }

  Serial.println("[MEM] 96 KiB staging OK AFTER SD mount");
  printHeap("after SD mount + staging");

  testNVS();

  Serial.println();
  Serial.println("==========================================");
  Serial.println(" RESULT: SD + FULL PLAYER MEMORY PASS");
  Serial.println("==========================================");
  Serial.println("The proven SD init path survives the full");
  Serial.println("player memory footprint.");
  Serial.println("No player/CBP code was executed.");

  while (true)
    delay(1000);
}

void loop()
{
  delay(1000);
}