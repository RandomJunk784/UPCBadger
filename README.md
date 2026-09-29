# UPCBadger / ConsoleBadger

Updated: 2026-09-29

## Active development — v2.34

Latest controlled gamerpic decoder experiment, based on the protected v2.31 physical recovery point.

### v2.34
- Keeps the proven Xbox 208x208 gamerpic source.
- Uses the recovered v1.85 static-Pngle architecture inside the existing 64 KiB animation workspace.
- Streams the PNG and bilinearly downsamples 208x208 -> 127px circular presentation.
- No 127x127 framebuffer is allocated.
- Existing R/B correction and product/UI/network/animation architecture remain unchanged.
- v2.32 direct 208->127 and v2.33 direct 208->115 LovyanGFX scaled-sprite tests failed at PNG decode.

### Protected recovery
- v2.31 remains the protected physical rollback point.
- v2.31 recovery snapshot: 02_KNOWN_GOOD/RECOVERY_V2.31_2026-09-29/

## Secret handling

Real API keys must stay out of GitHub.

Use local-only secrets and safe templates. Never commit a live OpenXBL API key.

## Safety

Before wiring, soldering, or physical modification:

**UNPLUG THE ESP32 FIRST.**
