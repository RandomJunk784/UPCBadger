# UPCBadger v2.34 — Streaming Gamerpic Test

- Based directly on protected physical recovery v2.31.
- v2.32 direct 208->127 LovyanGFX decode failed.
- v2.33 direct 208->115 LovyanGFX decode failed.
- v2.34 uses the recovered v1.85 static-Pngle architecture with the existing 64 KiB animation workspace as temporary decoder storage.
- Streams the proven 208x208 PNG and bilinearly downsamples it to the requested 127px circular presentation.
- No 127x127 framebuffer is allocated.
- Existing R/B correction, UI, Wi-Fi, XBL cadence, 20-minute profile sync, 12 FPS animation, SD/CBP and TLS/memory architecture are otherwise unchanged.
- v2.31 remains the physical rollback point.

**Not yet physically verified.**
