# UPCBadger

ConsoleBadger digital console badge project.

## Current working state
330x350 quality-ramp boot.
- 360x360 GC9B72 panel
- 330x350 active boot window
- 96 -> 192 -> 256 colour quality ramp
- LZ4 HC9
- direct-DMA playback path
- verified playback around 13.4 FPS
- best confirmed quality/performance baseline

## Product branch
- no dedicated phone app
- rear CONFIG button on GPIO32
- local ESP32 setup portal
- Wi-Fi + Xbox Gamertag only for first configuration
- settings stored locally in NVS
- no user database
- no persistent MAC customer records
- Xbox profile data via a small Cloudflare Worker -> OpenXBL service
- OpenXBL secret remains server-side
- 360x360 Xbox-themed profile UI
- 5-minute retention protection with fade, black interval and small positional shift

See docs/PRODUCT_ARCHITECTURE_V1.md, docs/PROFILE_UI_V1.md and services/profile-worker/.

## Standalone demo
firmware/current/UPCBadger_PROFILE_DEMO_V1_STANDALONE.ino

This deliberately does not use SD/CBP playback. It exists to prove provisioning and profile UI without risking the known-good animation path.