# UPCBadger / ConsoleBadger

Updated: 2026-09-29

## Active development — v2.36

Current gamerpic quality experiment, based on the protected physical v2.31 path.

### v2.36
- Keeps the proven Xbox 208x208 gamerpic source.
- Uses LovyanGFX 1.2.29 public Pngle interfaces.
- Performs true area-weighted 208x208 -> 127px RGB resampling, one output row at a time.
- Fixes the v2.35 Arduino auto-prototype failure by forward-declaring GamerPicAreaContext and gamerPicEmitReadyRows before the full context definition.
- No 72x72 intermediate and no 127x127 framebuffer.
- No intentional UI, Wi-Fi, XBL, SD/CBP, TLS/staging, animation timing or display changes.
- v2.31 remains the protected physical rollback point.

## Secret handling

Real API keys must stay out of GitHub.

Use local-only secrets and safe templates. Never commit a live OpenXBL API key.

## Safety

Before wiring, soldering, or physical modification:

**UNPLUG THE ESP32 FIRST.**
