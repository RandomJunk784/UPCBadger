# UPCBadger / ConsoleBadger

Updated: 2026-09-28

## Active development — v2.28

Latest firmware revision prepared from the compiled and physically working v2.27 baseline.

### v2.28 changes

- GamerID increased to approximately 18 px Orbitron.
- GamerScore remains 24 px Orbitron.
- Status bars retain the requested logical colour #9AF52A with corrected RGB565 packing for the direct DMA compositor.
- Gamerpic outer green ring remains removed.
- Gamerpic source/quality path remains unchanged.
- Profile data refresh model:
  - GamerTag and GamerScore: XBL profile sync every 20 minutes.
  - Wi-Fi/XBL status indicators: checked every 60 seconds.
  - Animation remains 12 FPS.
- Static profile UI is not redrawn every animation frame.
- The animation compositor retains only the necessary pixel protection without the previous large rectangular text boxes.

### v2.27 physical baseline

V2.27 is the first confirmed compile-and-run baseline for the static profile compositor architecture.

### Protected hardware/software architecture

Do not change without evidence:

- 181-frame / 12 FPS master animation
- TFT geometry and pins
- SD pins and requested 40 MHz operation
- TFT 80 MHz operation
- 32 KiB staging allocation and TLS memory-suspension ordering
- NVS setup portal
- background Wi-Fi startup
- XBL/OpenXBL transport
- gamerpic source/decode path

### Development rule

Subsequent revisions should change one controlled element at a time and be physically compiled/tested before becoming the new baseline.

## Secret handling

Real API keys must stay out of GitHub.

Use local-only secrets and safe templates. Never commit a live OpenXBL API key.

## Safety

Before wiring, soldering, or physical modification:

**UNPLUG THE ESP32 FIRST.**
