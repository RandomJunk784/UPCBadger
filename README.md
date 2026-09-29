# UPCBadger / ConsoleBadger

Updated: 2026-09-29

## Active development — v2.35

Current gamerpic quality experiment, based directly on the protected physical v2.31 recovery path.

### v2.35 area resampling
- Keeps the proven 208x208 Xbox gamerpic source.
- Keeps the proven LovyanGFX PNG decoder architecture.
- Uses LovyanGFX's public Pngle API to receive native decoded source pixels.
- Performs true 208x208 -> 127px area-weighted resampling in RGB888 space, one output row at a time.
- No 72x72 framebuffer.
- No 127x127 framebuffer.
- Existing R/B correction is applied at final RGB565 packing.
- 127px circular presentation retained.
- No intentional changes to UI, Wi-Fi, XBL timing, SD/CBP, TLS/staging recovery or 12 FPS animation.

v2.32 and v2.33 direct scaled-sprite tests failed at PNG decode. The current v2.35 uses the verified lgfx_pngle_* interface from the installed LovyanGFX 1.2.29 family.

### Protected recovery
v2.31 remains the physical rollback point.

## Secret handling

Real API keys must stay out of GitHub.

Use local-only secrets and safe templates. Never commit a live OpenXBL API key.

## Safety

Before wiring, soldering, or physical modification:

**UNPLUG THE ESP32 FIRST.**
