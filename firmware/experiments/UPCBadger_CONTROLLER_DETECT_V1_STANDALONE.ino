/*
  UPCBadger CONTROLLER DETECT V1 — STANDALONE
  --------------------------------------------
  Purpose:
    Experimental proof-of-concept for non-connecting gaming-controller
    detection on the original ESP32 DevKit 1.

  What this prototype does:
    - Uses the locked UPCBadger GC9B72 TFT pinout.
    - Scans BLE advertisements and logs identifying fields.\n    - Scans Bluetooth Classic inquiry results and logs names/COD/RSSI.
    - Classifies likely platform: XBOX / PLAYSTATION / NINTENDO / UNKNOWN.
    - Displays the currently recognised platform and intended boot theme.
    - Reads the existing local Xbox Gamertag key ("gt") from the profile demo
      NVS namespace so the profile side can be connected later.
    - DOES NOT connect to controllers, send commands, or store controller MACs.
    - DOES NOT modify the known-good boot/profile firmware.

  IMPORTANT:
    This is an EXPERIMENT, not yet a final platform detector.
    A controller may not advertise while connected, may randomise its address,
    or may expose different information depending on firmware/pairing state.
    The purpose is to discover what the real hardware actually broadcasts.

  Expected test sequence:
    1. Flash this standalone sketch to the original ESP32 DevKit 1.
    2. Open Serial Monitor at 115200.
    3. Leave all controllers off and note the baseline.
    4. Wake one controller at a time.
    5. Repeat several times and compare the reported fields.
    6. Record which values stay stable enough to become a platform fingerprint.

  SAFETY:
    UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.
*/

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <Preferences.h>

#if defined(CONFIG_BT_ENABLED) || defined(ARDUINO_ARCH_ESP32)
  #include <BLEDevice.h>
  #include <BLEScan.h>
  #include <BLEAdvertisedDevice.h>
  #include <BluetoothSerial.h>
#endif

// -------------------- Locked TFT pins --------------------
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   21
#define TFT_DC   22
#define TFT_RST  4
#define TFT_SPI_HZ 80000000UL

// -------------------- Timing --------------------
static const uint32_t BLE_SCAN_SECONDS = 3;
static const uint32_t BLE_SCAN_PERIOD_MS = 8500;
static const uint32_t DISPLAY_HOLD_MS = 4500;
static const uint32_t BOOT_THEME_CONFIRM_MS = 1200;

// -------------------- UI colours --------------------
static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
  return (uint16_t)(((r & 0xF8) << 8) |
                    ((g & 0xFC) << 3) |
                    (b >> 3));
}

static const uint16_t BLACK = 0x0000;
static const uint16_t WHITE = 0xFFFF;
static uint16_t xboxGreen()  { return rgb565(0, 255, 45); }
static uint16_t green2()     { return rgb565(0, 175, 35); }
static uint16_t dimGreen()   { return rgb565(0, 85, 18); }
static uint16_t amber()      { return rgb565(255, 180, 0); };
static uint16_t blue()       { return rgb565(70, 160, 255); };
static uint16_t red()        { return rgb565(255, 70, 70); };
static uint16_t purple()     { return rgb565(190, 110, 255); };

// -------------------- LovyanGFX --------------------
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
    cfg.pin_dc = TFT_DC;
    cfg.dma_channel = SPI_DMA_CH_AUTO;
    bus.config(cfg);
    panel.setBus(&bus);

    auto pc = panel.config();
    pc.pin_cs = TFT_CS;
    pc.pin_rst = TFT_RST;
    pc.pin_busy = -1;
    pc.panel_width = 360;
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

static LGFX tft;
static Preferences prefs;

#if defined(CONFIG_BT_ENABLED)
static BluetoothSerial btClassic;
#endif

// -------------------- Data model --------------------
enum Platform : uint8_t
{
  PLATFORM_UNKNOWN = 0,
  PLATFORM_XBOX,
  PLATFORM_PLAYSTATION,
  PLATFORM_NINTENDO
};

struct Detection
{
  Platform platform = PLATFORM_UNKNOWN;
  String source = "";
  String name = "";
  String address = "";
  String fingerprint = "";
  String details = "";
  int rssi = -127;
  uint32_t seenMs = 0;
  uint32_t hits = 0;
  bool valid = false;
};

static Detection lastDetection;
static String xboxGamertag;
static uint32_t lastBleScanMs = 0;
static uint32_t lastClassicScanMs = 0;
static bool bleReady = false;
static bool classicReady = false;
static bool scanBusy = false;

// -------------------- Helpers --------------------
static String upperCopy(String s)
{
  s.toUpperCase();
  return s;
}

static String bytesToHex(const uint8_t *data, size_t len, size_t maxBytes = 48)
{
  if (!data || len == 0) return "";
  String out;
  const size_t n = min(len, maxBytes);
  char b[4];

  for (size_t i = 0; i < n; ++i)
  {
    snprintf(b, sizeof(b), "%02X", data[i]);
    if (i) out += ' ';
    out += b;
  }

  if (len > n) out += " ...";
  return out;
}

static String platformName(Platform p)
{
  switch (p)
  {
    case PLATFORM_XBOX:       return "XBOX";
    case PLATFORM_PLAYSTATION:return "PLAYSTATION";
    case PLATFORM_NINTENDO:   return "NINTENDO";
    default:                  return "UNKNOWN";
  }
}

static uint16_t platformColour(Platform p)
{
  switch (p)
  {
    case PLATFORM_XBOX:        return xboxGreen();
    case PLATFORM_PLAYSTATION: return blue();
    case PLATFORM_NINTENDO:    return red();
    default:                   return amber();
  }
}

static Platform classifyName(String name)
{
  String n = upperCopy(name);

  if (n.indexOf("XBOX") >= 0)
    return PLATFORM_XBOX;

  if (n.indexOf("DUALSENSE") >= 0 ||
      n.indexOf("DUALSHOCK") >= 0 ||
      n.indexOf("PLAYSTATION") >= 0 ||
      n.indexOf("PS5") >= 0 ||
      n.indexOf("PS4") >= 0)
    return PLATFORM_PLAYSTATION;

  if (n.indexOf("PRO CONTROLLER") >= 0 ||
      n.indexOf("NINTENDO") >= 0 ||
      n.indexOf("JOY-CON") >= 0 ||
      n.indexOf("JOYCON") >= 0)
    return PLATFORM_NINTENDO;

  return PLATFORM_UNKNOWN;
}

static void maskAddressForScreen(String address, String &out)
{
  if (address.length() < 5)
  {
    out = address;
    return;
  }

  // Keep only the final four hex characters visible on the TFT.
  out = "••:" + address.substring(address.length() - 5);
}

static void loadExistingProfile()
{
  prefs.begin("upcbadger", true);
  xboxGamertag = prefs.getString("gt", "");
  prefs.end();

  if (xboxGamertag.length() == 0)
    xboxGamertag = "PROFILE NOT SET";
}

static void printDetection(const Detection &d)
{
  Serial.println();
  Serial.println(F("===================================================="));
  Serial.println(F("UPCBADGER CONTROLLER DETECTION"));
  Serial.println(F("===================================================="));
  Serial.printf("SOURCE       : %s\n", d.source.c_str());
  Serial.printf("PLATFORM     : %s\n", platformName(d.platform).c_str());
  Serial.printf("NAME         : %s\n", d.name.c_str());
  Serial.printf("ADDRESS      : %s\n", d.address.c_str());
  Serial.printf("RSSI         : %d dBm\n", d.rssi);
  Serial.printf("FINGERPRINT  : %s\n", d.fingerprint.c_str());
  Serial.printf("DETAILS      : %s\n", d.details.c_str());
  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("NOTE: controller addresses are displayed for testing"));
  Serial.println(F("only and are NOT stored by this firmware."));
  Serial.println(F("===================================================="));
}

static void setDetection(Platform platform,
                         const String &source,
                         const String &name,
                         const String &address,
                         const String &fingerprint,
                         const String &details,
                         int rssi)
{
  const bool sameFingerprint =
      lastDetection.valid &&
      fingerprint.length() > 0 &&
      fingerprint == lastDetection.fingerprint &&
      platform == lastDetection.platform;

  lastDetection.platform = platform;
  lastDetection.source = source;
  lastDetection.name = name.length() ? name : "(no name)";
  lastDetection.address = address;
  lastDetection.fingerprint = fingerprint;
  lastDetection.details = details;
  lastDetection.rssi = rssi;
  lastDetection.seenMs = millis();
  lastDetection.hits = sameFingerprint ? lastDetection.hits + 1 : 1;
  lastDetection.valid = true;

  printDetection(lastDetection);
}

// -------------------- TFT helpers --------------------
static void setText(const String &s, int x, int y, uint16_t colour,
                    const lgfx::IFont *font,
                    textdatum_t datum = textdatum_t::middle_center)
{
  tft.setTextDatum(datum);
  tft.setFont(font);
  tft.setTextColor(colour, BLACK);
  tft.drawString(s, x, y);
}

static void drawArcBadge(Platform p)
{
  const uint16_t c = platformColour(p);
  tft.drawCircle(180, 180, 156, dimGreen());
  tft.drawCircle(180, 180, 150, c);

  if (p == PLATFORM_XBOX)
  {
    tft.drawLine(144, 132, 170, 158, c);
    tft.drawLine(216, 132, 190, 158, c);
    tft.drawLine(144, 228, 180, 180, c);
    tft.drawLine(216, 228, 180, 180, c);
  }
  else if (p == PLATFORM_PLAYSTATION)
  {
    tft.drawLine(150, 220, 180, 138, c);
    tft.drawLine(180, 138, 210, 220, c);
    tft.drawLine(158, 198, 202, 198, c);
  }
  else if (p == PLATFORM_NINTENDO)
  {
    tft.drawRect(150, 145, 60, 70, c);
    tft.fillCircle(164, 160, 4, c);
    tft.fillCircle(196, 160, 4, c);
  }
  else
  {
    tft.drawCircle(180, 180, 45, c);
    tft.drawLine(180, 135, 180, 225, c);
    tft.drawLine(135, 180, 225, 180, c);
  }
}

static void drawScanScreen()
{
  tft.fillScreen(BLACK);
  setText("UPCBadger", 180, 35, xboxGreen(), &fonts::Font4);
  setText("CONTROLLER SCAN", 180, 72, green2(), &fonts::Font2);

  if (!lastDetection.valid)
  {
    drawArcBadge(PLATFORM_UNKNOWN);
    setText("LISTENING...", 180, 260, amber(), &fonts::Font2);
    setText("BLE + CLASSIC", 180, 290, dimGreen(), &fonts::Font0);
    return;
  }

  drawArcBadge(lastDetection.platform);

  setText(platformName(lastDetection.platform), 180, 250,
          platformColour(lastDetection.platform), &fonts::Font2);

  String masked;
  maskAddressForScreen(lastDetection.address, masked);
  setText(masked, 180, 278, WHITE, &fonts::Font0);

  String theme = platformName(lastDetection.platform) + " THEME";
  setText(theme, 180, 305, green2(), &fonts::Font0);

  if (lastDetection.platform == PLATFORM_XBOX)
    setText(xboxGamertag, 180, 328, WHITE, &fonts::Font0);
  else
    setText("PROFILE LINK LATER", 180, 328, dimGreen(), &fonts::Font0);
}

static void drawBootThemePreview(Platform p)
{
  tft.fillScreen(BLACK);

  String title = platformName(p) + " BOOT";
  setText(title, 180, 85, platformColour(p), &fonts::Font4);
  setText("CONTROLLER DETECTED", 180, 120, green2(), &fonts::Font2);

  drawArcBadge(p);

  String sub;
  if (p == PLATFORM_XBOX)
    sub = "WOULD LOAD: XBOX";
  else if (p == PLATFORM_PLAYSTATION)
    sub = "WOULD LOAD: PLAYSTATION";
  else if (p == PLATFORM_NINTENDO)
    sub = "WOULD LOAD: NINTENDO";
  else
    sub = "WOULD LOAD: DEFAULT";

  setText(sub, 180, 285, WHITE, &fonts::Font0);
  setText("DEMO ONLY — NO BOOT ASSET CHANGED", 180, 320, dimGreen(), &fonts::Font0);
}

// -------------------- BLE scanning --------------------
class ScanCallbacks : public BLEAdvertisedDeviceCallbacks
{
public:
  void onResult(BLEAdvertisedDevice advertisedDevice) override
  {
    String name = advertisedDevice.haveName() ? advertisedDevice.getName() : "";
    String address = advertisedDevice.getAddress().toString().c_str();
    String mfg = advertisedDevice.haveManufacturerData()
                   ? advertisedDevice.getManufacturerData()
                   : "";
    String service = advertisedDevice.haveServiceUUID()
                   ? advertisedDevice.getServiceUUID().toString().c_str()
                   : "";

    Platform p = classifyName(name);

    // For the first experiment we deliberately keep the classification conservative.
    // A generic "Wireless Controller" does NOT get called PlayStation solely by name.
    if (p == PLATFORM_UNKNOWN && mfg.length())
    {
      // Keep raw manufacturer data as the fingerprint material.
      // Future experiments can map known vendor blocks once captured from real pads.
    }

    String fingerprint;
    fingerprint.reserve(220);
    fingerprint += "NAME=" + upperCopy(name);
    fingerprint += "|MFG=" + bytesToHex(
      reinterpret_cast<const uint8_t *>(mfg.c_str()), mfg.length());
    fingerprint += "|SVC=" + service;

    String details =
        String("MFG: ") + bytesToHex(
          reinterpret_cast<const uint8_t *>(mfg.c_str()), mfg.length()) +
        String(" | SVC: ") + service +
        String(" | AP: ") + (advertisedDevice.haveAppearance()
          ? String(advertisedDevice.getAppearance()) : String("n/a"));

    setDetection(p, "BLE", name, address, fingerprint, details,
                 advertisedDevice.haveRSSI() ? advertisedDevice.getRSSI() : -127);
  }
};

static ScanCallbacks bleCallbacks;

static void runBleScan()
{
  if (!bleReady || scanBusy) return;
  scanBusy = true;

  Serial.println();
  Serial.println(F(">>> BLE SCAN START"));

  BLEScan *scan = BLEDevice::getScan();
  if (!scan)
  {
    Serial.println(F("BLE scan object unavailable."));
    scanBusy = false;
    return;
  }

  scan->setAdvertisedDeviceCallbacks(&bleCallbacks, true);
  scan->setActiveScan(true);
  scan->setInterval(100);
  scan->setWindow(80);

  BLEScanResults results = scan->start(BLE_SCAN_SECONDS, false);
  (void)results;
  scan->clearResults();

  Serial.println(F(">>> BLE SCAN END"));
  scanBusy = false;
}

// -------------------- Bluetooth Classic scanning --------------------
#if defined(CONFIG_BT_ENABLED)

static void classicDeviceCallback(BTAdvertisedDevice *device)
{
  if (!device) return;

  const String name = device->haveName()
                    ? String(device->getName().c_str())
                    : String("");
  const String address = device->getAddress().toString().c_str();
  const uint32_t cod = device->haveCOD() ? device->getCOD() : 0;
  const int rssi = device->haveRSSI() ? device->getRSSI() : -127;

  Platform p = classifyName(name);

  char codText[16];
  snprintf(codText, sizeof(codText), "0x%06lX", (unsigned long)cod);

  String fingerprint = "NAME=" + upperCopy(name) + "|COD=" + String(codText);

  String details = String("COD: ") + codText +
                   String(" | RSSI: ") + String(rssi);

  setDetection(p, "CLASSIC", name, address, fingerprint, details, rssi);
}

static void runClassicScan()
{
  if (!classicReady || scanBusy) return;
  scanBusy = true;

  Serial.println();
  Serial.println(F(">>> BLUETOOTH CLASSIC INQUIRY START"));

  btClassic.discoverAsync(classicDeviceCallback, 6000);
  delay(6200);
  btClassic.discoverAsyncStop();
  btClassic.discoverClear();

  Serial.println(F(">>> BLUETOOTH CLASSIC INQUIRY END"));
  scanBusy = false;
}

#endif

// -------------------- Setup / loop --------------------
void setup()
{
  Serial.begin(115200);
  delay(300);

  Serial.println();
  Serial.println(F("=============================================="));
  Serial.println(F(" UPCBADGER CONTROLLER DETECT V1"));
  Serial.println(F(" PASSIVE FINGERPRINT EXPERIMENT"));
  Serial.println(F("=============================================="));
  Serial.println(F("No controller connections are attempted."));
  Serial.println(F("Controller addresses are NOT persisted."));

  tft.init();
  tft.setRotation(0);
  tft.setColorDepth(16);
  tft.fillScreen(BLACK);
  drawScanScreen();

  loadExistingProfile();

#if defined(CONFIG_BT_ENABLED)
  // Classic only: leave BLE under the separate BLE API.
  classicReady = btClassic.begin("UPCBadgerScanner", false, true);
  Serial.printf("Bluetooth Classic init: %s\n",
                classicReady ? "OK" : "FAILED");
#endif

#if defined(SOC_BLE_SUPPORTED) || defined(CONFIG_BT_ENABLED)
  bleReady = BLEDevice::init("UPCBadgerScanner");
  Serial.printf("BLE init: %s\n", bleReady ? "OK" : "FAILED");
#endif

  if (!classicReady && !bleReady)
  {
    tft.fillScreen(BLACK);
    setText("BLUETOOTH ERROR", 180, 145, red(), &fonts::Font2);
    setText("CHECK ESP32 BOARD", 180, 185, WHITE, &fonts::Font0);
    while (true) delay(1000);
  }

  Serial.println();
  Serial.println(F("Ready. Wake ONE controller at a time."));
  Serial.println(F("Repeat each wake-up several times to find stable fingerprints."));
  Serial.printf("Existing Xbox Gamertag: %s\n", xboxGamertag.c_str());
}

void loop()
{
  // BLE pass first.
  if (bleReady && millis() - lastBleScanMs >= BLE_SCAN_PERIOD_MS && !scanBusy)
  {
    lastBleScanMs = millis();
    runBleScan();

    if (lastDetection.valid)
    {
      drawScanScreen();
      delay(BOOT_THEME_CONFIRM_MS);
      drawBootThemePreview(lastDetection.platform);
      delay(DISPLAY_HOLD_MS);
      drawScanScreen();
    }
  }

#if defined(CONFIG_BT_ENABLED)
  // Then Classic inquiry. This is intentionally periodic rather than continuous.
  if (classicReady && millis() - lastClassicScanMs >= 14000UL && !scanBusy)
  {
    lastClassicScanMs = millis();
    runClassicScan();

    if (lastDetection.valid)
    {
      drawScanScreen();
      delay(BOOT_THEME_CONFIRM_MS);
      drawBootThemePreview(lastDetection.platform);
      delay(DISPLAY_HOLD_MS);
      drawScanScreen();
    }
  }
#endif

  delay(20);
}
