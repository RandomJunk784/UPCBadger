BPBadger Lily S3 v0.05.4 FLASH RGB — FULL POWER

Base:
  BPBadger Lily S3 v0.04 FLASH known-good build.

Storage:
  Internal LittleFS only. No SD card.
  CBP asset: Lily_Boot_Animation_12FPS.CBP
  Expected CBP: 10,438,338 bytes, 122 frames, 360x360, 12 FPS.

Display:
  GC9B72 / LovyanGFX configuration retained from known-good v0.03 RGB-fixed build.
  TFT SDA/MOSI = GPIO17
  TFT SCL/SCLK = GPIO18
  TFT CS       = GPIO21
  TFT DC       = GPIO16
  TFT RST      = GPIO4
  TFT VCC      = 3V3
  TFT BL       = 3V3

Accelerometer:
  LIS3DH SDA  = GPIO9
  LIS3DH SCL  = GPIO8
  LIS3DH INT1 = GPIO7

RGB:
  Onboard addressable RGB LED = GPIO48.
  Full-power pink/purple alternating movement indicator.
  Pink   = 255,0,180
  Purple = 180,0,255

Behaviour:
  Movement triggers RGB and resets the existing 60-second active timer.
  RGB is turned off before deep sleep.
  LIS3DH wake restarts the active state and RGB indication.

Diagnostics:
  Lightweight [INFO] and [CRASH] messages retained.
  No visual test patterns, GPIO test sweeps, SD tests, Wi-Fi, XBL or API code.

Arduino:
  Use the supplied partitions_16MB_LittleFS.csv as the Custom partition scheme.
  Upload the firmware first, then upload the supplied data folder with LittleFS.

v0.05.4 compile fix:
  StageReader remains defined before every function using it. The RGB helper
  functions are placed after the StageReader type so Arduino .ino prototype
  generation cannot emit StageReader-dependent prototypes before the type.


RGB behaviour: onboard RGB LED on GPIO48. While the badge is active, the LED remains on continuously and alternates full-power pink (255,0,180) and purple (180,0,255) every 5 seconds. It is switched off on the sleep event.