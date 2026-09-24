# UPCBadger

ConsoleBadger digital console badge project.

## Current engineering state

The integrated player/product path is being brought together.

### Proven
- 360x360 GC9B72 panel
- 330x350 active boot window
- 96 → 192 → 256 colour quality ramp
- LZ4 HC9 / CBP player
- direct-DMA playback path
- verified playback around 13.4 FPS on the protected performance baseline
- SD + NVS + Wi-Fi compatibility path
- v1.14: SD initialisation and CBP index loading succeed with the full player globals present

### Current blocker
v1.14 reaches:

`SD.begin() OK`
`Xbox CBP index loaded.`

but cannot allocate the 96 KiB playback staging buffer afterwards.

The next revision will target the staging allocation while preserving the proven player and product flow.

## Product direction
- rear CONFIG button on GPIO32
- local ESP32 setup portal
- Wi-Fi + Xbox Gamertag for first configuration
- settings stored locally in NVS
- no user database
- no persistent MAC customer records
- Xbox profile data via Cloudflare Worker → OpenXBL
- 360x360 Xbox-themed profile UI
- retention protection for the long-lived profile screen

## Repository structure

See:
- `docs/PRODUCT_ARCHITECTURE_V1.md`
- `docs/PROFILE_UI_V1.md`
- `docs/UPCBadger_HANDOFF_2026-09-24.md`
- `services/profile-worker/`

## Standalone demo

`firmware/current/UPCBadger_PROFILE_DEMO_V1_STANDALONE.ino`

This deliberately avoids SD/CBP playback so provisioning/profile UI can be tested independently.
