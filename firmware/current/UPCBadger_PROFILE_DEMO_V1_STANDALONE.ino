/*
  UPCBadger PROFILE DEMO V1 — STANDALONE
  -------------------------------------
  Purpose:
    Prove the new product layer without touching the proven CBP/SD playback.

  Features:
    - 360x360 GC9B72 on the locked TFT pins
    - browser-based first-time setup over ESP32 SoftAP
    - captive-DNS portal + direct 192.168.4.1 fallback
    - Wi-Fi SSID/password stored locally in NVS
    - Xbox Gamer ID stored locally in NVS
    - rear CONFIG button reserved on GPIO32, internal pull-up
    - Xbox-themed profile UI matching the UPCBadger concept
    - 5-minute retention protection with fade/black/2px shift

  IMPORTANT:
    This is intentionally STANDALONE. It does not use the SD card or CBP boot
    animation yet. The live OpenXBL request is deliberately deferred until the
    cloud Worker is deployed; the UI uses clearly labelled demo data meanwhile.

  SAFETY:
    UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.
*/

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

// -------------------- Locked TFT pins --------------------
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   21
#define TFT_DC   22
#define TFT_RST   4
#define TFT_SPI_HZ 80000000UL

// -------------------- Config button --------------------
#define CONFIG_BUTTON_PIN 32
#define CONFIG_HOLD_MS 3000UL
#define FACTORY_RESET_HOLD_MS 15000UL

// -------------------- Retention --------------------
#define RETENTION_INTERVAL_MS (5UL * 60UL * 1000UL)
#define BLACK_HOLD_MS 1000UL
#define FADE_STEPS 8
#define FADE_STEP_MS 70UL

// -------------------- Theme --------------------
static const uint16_t BLACK = 0x0000;
static const uint16_t WHITE = 0xFFFF;

static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
  return (uint16_t)(((r & 0xF8) << 8) |
                    ((g & 0xFC) << 3) |
                    (b >> 3));
}

static uint16_t scale565(uint16_t c, uint8_t amount)
{
  if (amount == 255) return c;
  uint32_t r = (c >> 11) & 0x1F;
  uint32_t g = (c >> 5)  & 0x3F;
  uint32_t b = c & 0x1F;
  r = (r * amount + 127) / 255;
  g = (g * amount + 127) / 255;
  b = (b * amount + 127) / 255;
  return (uint16_t)((r << 11) | (g << 5) | b);
}

static uint16_t green(uint8_t a = 255) { return scale565(rgb565(0,255,45), a); }
static uint16_t green2(uint8_t a = 255) { return scale565(rgb565(0,175,35), a); }
static uint16_t green3(uint8_t a = 255) { return scale565(rgb565(0,85,18), a); }
static uint16_t softWhite(uint8_t a = 255) { return scale565(rgb565(215,225,220), a); }

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
static WebServer server(80);
static DNSServer dns;
static Preferences prefs;

// -------------------- Persistent settings --------------------
static String wifiSSID;
static String wifiPassword;
static String gamertag;

// Temporary setup-network identity shown on the TFT and used by the AP.
static String setupApSSID;

// -------------------- Runtime state --------------------
static bool configMode = false;
static bool buttonLatched = false;
static uint32_t buttonDownMs = 0;
static uint32_t screenStartedMs = 0;
static uint8_t protectionShift = 0;

// Demo data is deliberately labelled on-screen until live API is attached.
static uint32_t demoGamerscore = 125640;

// -------------------- UI helpers --------------------
static String initials(String name)
{
  name.trim();
  name.toUpperCase();
  if (name.length() == 0) return "X";
  if (name.length() > 2) name = name.substring(0, 2);
  return name;
}

static void text(const String &s, int x, int y, uint16_t color,
                 const lgfx::IFont *font, textdatum_t datum = textdatum_t::middle_center)
{
  tft.setTextDatum(datum);
  tft.setFont(font);
  tft.setTextColor(color, BLACK);
  tft.drawString(s, x, y);
}

static void cleanPixelText(const String &s, int x, int y, uint16_t color,
                           uint8_t scale = 2,
                           textdatum_t datum = textdatum_t::middle_center)
{
  tft.setTextDatum(datum);
  tft.setFont(&fonts::Font0);
  tft.setTextSize(scale);
  tft.setTextColor(color, BLACK);
  tft.drawString(s, x, y);
  tft.setTextSize(1);
}

static void drawWifiIcon(int x, int y, uint8_t a)
{
  uint16_t c = green(a);
  tft.fillCircle(x, y + 10, 3, c);
  tft.drawArc(x, y + 10, 10, 10, 225, 315, c);
  tft.drawArc(x, y + 10, 17, 17, 225, 315, c);
}

static void drawProfile(uint8_t brightness, int shift)
{
  uint16_t g = green(brightness);
  uint16_t g2 = green2(brightness);
  uint16_t g3 = green3(brightness);
  uint16_t w = softWhite(brightness);
  int cx = 180 + shift;
  int cy = 176;

  tft.fillScreen(BLACK);

  tft.drawCircle(cx, cy, 174, g3);
  tft.drawCircle(cx, cy, 170, g3);

  text("UPCBadger", cx, 24, g2, &fonts::Font0);
  text("PROFILE", cx, 48, g, &fonts::Font2);

  tft.drawCircle(cx, cy - 38, 64, g3);
  tft.drawCircle(cx, cy - 38, 58, g);
  tft.fillCircle(cx, cy - 38, 53, BLACK);
  text(initials(gamertag), cx, cy - 38, g, &fonts::Font4);

  text(gamertag.length() ? gamertag : "PLAYER", cx, cy + 50, w, &fonts::Font4);
  tft.drawFastHLine(70 + shift, cy + 77, 220, g3);

  text("GAMERSCORE", cx, cy + 97, g2, &fonts::Font0);
  text(String(demoGamerscore), cx, cy + 123, w, &fonts::Font4);

  tft.fillCircle(74 + shift, 333, 5, g);
  text("PROFILE DEMO", 89 + shift, 333, g2, &fonts::Font0, textdatum_t::middle_left);

  drawWifiIcon(287 + shift, 320, brightness);
  text(WiFi.status() == WL_CONNECTED ? "WI-FI" : "OFFLINE",
       311 + shift, 337, g3, &fonts::Font0, textdatum_t::middle_left);

  tft.setTextDatum(textdatum_t::middle_center);
}

static void drawBoot(float phase)
{
  uint8_t a = (uint8_t)(80 + 175.0f * phase);
  uint16_t g = green(a);
  uint16_t g2 = green2(a);

  tft.fillScreen(BLACK);
  tft.drawCircle(180, 150, 92, green3(a));
  tft.drawCircle(180, 150, 80, g);
  tft.drawCircle(180, 150, 76, green3(a));

  // Stylised X.
  tft.drawLine(145, 115, 170, 150, g);
  tft.drawLine(215, 115, 190, 150, g);
  tft.drawLine(145, 185, 180, 150, g);
  tft.drawLine(215, 185, 180, 150, g);
  tft.drawLine(145, 116, 180, 150, g2);
  tft.drawLine(215, 116, 180, 150, g2);

  text("XBOX", 180, 238, softWhite(a), &fonts::Font2);
  text("PLAY YOUR WAY", 180, 263, g2, &fonts::Font0);
}

static void bootSequence()
{
  const uint32_t total = 2800;
  const uint32_t fade = 900;
  uint32_t start = millis();

  while (millis() - start < total)
  {
    uint32_t e = millis() - start;
    if (e < 1900)
    {
      float p = e / 1900.0f;
      drawBoot(p);
    }
    else
    {
      uint32_t f = e - 1900;
      uint8_t a = f >= fade ? 0 : (uint8_t)(255UL - (f * 255UL / fade));
      drawBoot(a / 255.0f);
    }
    delay(45);
  }

  tft.fillScreen(BLACK);
}

static void drawSetupScreen()
{
  tft.fillScreen(BLACK);

  const int cx = 180;

  tft.setTextDatum(textdatum_t::middle_center);
  tft.setFont(&fonts::Font0);
  tft.setTextSize(2);

  tft.setTextColor(softWhite(), BLACK);
  tft.drawString("ConsoleBadger Setup", cx, 95);

  tft.setTextColor(green(), BLACK);
  tft.drawString("Open 192.168.4.1", cx, 150);

  tft.setTextColor(softWhite(), BLACK);
  tft.drawString("using your phone browser", cx, 195);

  tft.setTextColor(green2(), BLACK);
  tft.drawString("(Turn OFF mobile data)", cx, 255);

  tft.setTextSize(1);
}

static const char CONFIG_HTML[] PROGMEM = R"HTML(
<!doctype html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<title>ConsoleBadger Setup</title>
<style>
body{margin:0;background:#050806;color:#e9fff0;font-family:Arial,sans-serif}.wrap{max-width:430px;margin:auto;padding:24px}.card{background:#07110a;border:1px solid #1b5a2a;border-radius:22px;padding:22px;box-shadow:0 0 30px #00ff4420}h1{margin:0;color:#15ff4d;font-size:30px}p{color:#79a985}label{display:block;margin-top:18px;color:#9de7ae;font-size:14px}input{box-sizing:border-box;width:100%;padding:14px;margin-top:7px;border-radius:12px;border:1px solid #234c2c;background:#020503;color:white;font-size:16px}button{width:100%;padding:15px;margin-top:24px;border:0;border-radius:12px;background:#15dc48;color:#001b07;font-weight:bold;font-size:16px}.small{font-size:12px;color:#577e60;margin-top:18px}
</style></head><body><div class="wrap"><div class="card"><h1>Console<span style="color:#11dc49">Badger</span></h1><p>Configure your badge.</p><form method="POST" action="/save"><label>Wi-Fi network<input name="ssid" maxlength="64" autocomplete="off" required></label><label>Wi-Fi password<input name="password" type="password" maxlength="64" autocomplete="off"></label><label>Xbox Gamer ID<input name="gamertag" maxlength="32" autocomplete="off" required></label><button>SAVE &amp; CONNECT</button></form><div class="small">Wi-Fi and Gamertag are saved on the badge.</div></div></div></body></html>
)HTML";

static void handleRoot()
{
  server.send(200, "text/html", CONFIG_HTML);
}

static void handleSave()
{
  String ssid = server.arg("ssid");
  String pass = server.arg("password");
  String gt = server.arg("gamertag");
  ssid.trim();
  gt.trim();

  if (ssid.isEmpty() || gt.isEmpty())
  {
    server.send(400, "text/plain", "Wi-Fi network and Gamertag are required.");
    return;
  }

  prefs.begin("upcbadger", false);
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.putString("gt", gt);
  prefs.end();

  server.send(200, "text/html", "<html><body style='font-family:Arial;background:#050806;color:white;text-align:center;padding:40px'><h1 style='color:#15ff4d'>Saved.</h1><p>Restarting UPCBadger...</p></body></html>");
  delay(900);
  ESP.restart();
}

static void startConfigMode()
{
  configMode = true;

  // Open temporary setup Wi-Fi. No password is required.
  WiFi.mode(WIFI_AP);
  WiFi.softAPdisconnect(true);
  delay(100);

  setupApSSID = "ConsoleBadger";

  IPAddress ip(192,168,4,1);
  IPAddress mask(255,255,255,0);
  WiFi.softAPConfig(ip, ip, mask);

  if (!WiFi.softAP(setupApSSID.c_str(), nullptr, 6, false, 1))
  {
    Serial.println("ERROR: setup AP start failed");
    while (true) delay(1000);
  }

  dns.start(53, "*", ip);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/generate_204", HTTP_GET, handleRoot);
  server.on("/hotspot-detect.html", HTTP_GET, handleRoot);
  server.on("/connecttest.txt", HTTP_GET, handleRoot);
  server.onNotFound([](){
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  });
  server.begin();
  drawSetupScreen();
}

static bool loadSettings()
{
  prefs.begin("upcbadger", true);
  wifiSSID = prefs.getString("ssid", "");
  wifiPassword = prefs.getString("pass", "");
  gamertag = prefs.getString("gt", "");
  prefs.end();
  return wifiSSID.length() > 0 && gamertag.length() > 0;
}

static bool connectWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("upcbadger");
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());

  tft.fillScreen(BLACK);
  text("UPCBadger", 180, 145, green(), &fonts::Font4);
  text("CONNECTING WI-FI", 180, 200, green2(), &fonts::Font0);

  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000UL)
    delay(100);

  if (WiFi.status() == WL_CONNECTED)
  {
    tft.fillScreen(BLACK);
    text("WI-FI CONNECTED", 180, 150, green(), &fonts::Font2);
    text(WiFi.localIP().toString(), 180, 195, softWhite(), &fonts::Font2);
    delay(1800);
    return true;
  }

  tft.fillScreen(BLACK);
  text("WI-FI FAILED", 180, 150, green2(), &fonts::Font2);
  text("RETURNING TO SETUP", 180, 195, softWhite(), &fonts::Font0);
  delay(1200);
  return false;
}

static void factoryResetAndSetup()
{
  Serial.println("FACTORY RESET REQUESTED");

  prefs.begin("upcbadger", false);
  prefs.clear();
  prefs.end();

  wifiSSID = "";
  wifiPassword = "";
  gamertag = "";

  tft.fillScreen(BLACK);
  text("UPCBadger", 180, 145, green(), &fonts::Font4);
  text("FACTORY RESET", 180, 195, green2(), &fonts::Font2);
  text("STARTING SETUP", 180, 230, green3(), &fonts::Font0);

  delay(1200);
  startConfigMode();
}

static void checkConfigButton()
{
  bool down = digitalRead(CONFIG_BUTTON_PIN) == LOW;

  if (down)
  {
    if (!buttonLatched)
    {
      buttonLatched = true;
      buttonDownMs = millis();
    }
    return;
  }

  if (!buttonLatched)
    return;

  uint32_t held = millis() - buttonDownMs;

  buttonLatched = false;
  buttonDownMs = 0;

  // Decide on release so a 15-second hold does not trigger
  // the 3-second setup action first.
  if (held >= FACTORY_RESET_HOLD_MS)
  {
    if (!configMode)
      factoryResetAndSetup();

    return;
  }

  if (held >= CONFIG_HOLD_MS && !configMode)
    startConfigMode();
}

static void retentionCycle()
{
  for (int step = FADE_STEPS; step >= 0; --step)
  {
    uint8_t a = (uint8_t)(255L * step / FADE_STEPS);
    drawProfile(a, protectionShift ? 2 : 0);
    delay(FADE_STEP_MS);
  }

  tft.fillScreen(BLACK);
  delay(BLACK_HOLD_MS);

  protectionShift ^= 1;
  for (uint8_t step = 0; step <= FADE_STEPS; ++step)
  {
    uint8_t a = (uint8_t)(255L * step / FADE_STEPS);
    drawProfile(a, protectionShift ? 2 : 0);
    delay(FADE_STEP_MS);
  }

  screenStartedMs = millis();
}

void setup()
{
  Serial.begin(115200);
  delay(200);
  pinMode(CONFIG_BUTTON_PIN, INPUT_PULLUP);

  if (!tft.init())
    while (true) delay(1000);

  tft.setRotation(0);
  tft.setColorDepth(16);
  tft.setSwapBytes(false);
  tft.initDMA();
  tft.fillScreen(BLACK);

  if (!loadSettings() || digitalRead(CONFIG_BUTTON_PIN) == LOW)
  {
    startConfigMode();
    return;
  }

  if (!connectWiFi())
  {
    startConfigMode();
    return;
  }

  Serial.print("Wi-Fi IP: ");
  Serial.println(WiFi.localIP());

  bootSequence();
  drawProfile(0, 0);
  for (uint8_t a = 0; a <= 255; a += 51)
  {
    drawProfile(a, 0);
    delay(75);
  }
  screenStartedMs = millis();
}

void loop()
{
  if (configMode)
  {
    dns.processNextRequest();
    server.handleClient();
    delay(5);
    return;
  }

  checkConfigButton();

  if (millis() - screenStartedMs >= RETENTION_INTERVAL_MS)
    retentionCycle();

  delay(30);
}