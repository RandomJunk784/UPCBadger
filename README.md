# UPCBadger

ConsoleBadger digital console badge project.

## Current working state — v1.17

PROTECTED WORKING BASE.

The integrated product path is now working on the project hardware:
- SD initialisation and CBP index load
- Xbox boot animation
- Gamer ID/profile screen
- background home Wi-Fi
- configuration portal
- Wi-Fi scan
- NVS save
- reboot and persisted settings restoration

The working video path is the 360x360 GC9B72 / 330x350 active window using the quality-ramp player architecture.

## Product direction
- no dedicated phone app
- rear CONFIG button on GPIO32
- local ESP32 setup portal
- Wi-Fi + Xbox Gamertag for first configuration
- settings stored locally in NVS
- no user database
- no persistent MAC customer records
- Xbox profile data via Cloudflare Worker -> OpenXBL service
- OpenXBL secret remains server-side
- 360x360 Xbox-themed profile UI
- 5-minute retention protection

## Current development target

Improve the static Gamer ID/profile screen visually while preserving the working playback and product flow. Live Xbox profile data retrieval comes after the static UI is satisfactory.

See:
- docs/PRODUCT_ARCHITECTURE_V1.md
- docs/PROFILE_UI_V1.md
- docs/UPCBadger_HANDOFF_2026-09-24.md
- docs/MILESTONE_V1.17_2026-09-24.md
- services/profile-worker/

## Standalone demo

firmware/current/UPCBadger_PROFILE_DEMO_V1_STANDALONE.ino

This deliberately avoids SD/CBP playback so provisioning/profile UI can be tested independently.