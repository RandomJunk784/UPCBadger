# UPCBadger

ConsoleBadger digital console badge project.

## Current development state — v1.38 / V28

**Prepared PC-verified test build. v1.36 remains the protected latest physically tested firmware.**

The current product path includes:
- Xbox/ConsoleBadger boot flow
- Gamer ID/profile screen
- background home Wi-Fi
- configuration portal and NVS save/persistence
- white ONLINE + Wi-Fi profile status row
- V28 single-layer Lumina Gamer ID background
- live profile overlay architecture ready for future Xbox API gamerpic/data

### V28 Lumina background

The Gamer ID background has been redesigned as one full-screen animated asset:
- source: 2026-09-26_11-08-26_Lumina.mp4
- 97 source frames retained
- 360x360 output
- 12 FPS
- ~8.08 second continuous loop
- one runtime CBP: `/profile_energy.CBP`
- no animated `profile_energy_patch.CBP` required
- fresh stable 256-colour RGB565 palette generated from the new Lumina animation
- no dithering in the current production candidate
- PC round-trip RMSE ~1.71 RGB levels/channel in expanded RGB565 display space

The V28 architecture removes the previous base+patch colour/compositor mismatch path.

### v1.38 profile renderer

Based directly on corrected v1.37:
- full-screen Lumina background is streamed continuously
- live Gamer ID overlay is restored after each frame
- profile animation runs at the established 12 FPS operating point
- frame scheduling is deadline-oriented to reduce timing drift
- no second animated patch asset is opened or required

Real Xbox API integration can later provide gamerpic, gamertag and status without changing the Lumina background.

### Important test status

v1.38 is **not yet physically verified**. Arduino IDE compile/upload and LCD testing are the next steps.

The first physical V28 test should answer:
1. Is the full-screen Lumina animation smooth at 12 FPS?
2. Does the live overlay remain visually stable?
3. Does the single-CBP architecture eliminate the previous horizontal patch seams?

If the live overlay flickers, the next change should be a protected overlay/no-write region rather than another colour transformation.

## Product direction

- local ESP32 setup portal
- Wi-Fi + Xbox Gamertag for first configuration
- settings stored locally in NVS
- no user database
- no persistent MAC customer records
- Xbox profile data via Cloudflare Worker -> OpenXBL service
- 360x360 Xbox-themed profile UI
- 5-minute retention protection as secondary burn/retention protection

## Evidence and provenance

The Library is the project source of truth for:
- historical versions
- large binary SD assets
- original/user-supplied media
- physical LCD video/photo evidence
- encoder investigation and reconstruction records
- checksums and PC validation evidence

The original CBP encoder source has not been recovered. `make_cbp1_reconstructed.py` is a forensic reconstruction and must not be treated as the historical original.

See:
- docs/PRODUCT_ARCHITECTURE_V1.md
- docs/PROFILE_UI_V1.md
- docs/evidence/2026-09-26-v27-colour-lcd-freeze.md
- docs/evidence/2026-09-26-v28-lumina-single-cbp.md
- docs/evidence/2026-09-26-v28-checksums.txt
- firmware/UPCBadger_v1.37_UI_CHANGE.md
- docs/MILESTONE_V1.17_2026-09-24.md
- services/profile-worker/

## Standalone demo

firmware/current/UPCBadger_PROFILE_DEMO_V1_STANDALONE.ino

This deliberately avoids SD/CBP playback so provisioning/profile UI can be tested independently.
