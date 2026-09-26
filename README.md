# UPCBadger

ConsoleBadger digital console badge project.

## Current working state — v1.37

**Prepared test build. v1.36 remains the protected latest physically tested firmware.**

The project currently has:
- Xbox/ConsoleBadger boot flow working
- Gamer ID/profile screen working
- V27 matched green colour treatment for profile energy assets
- background home Wi-Fi
- configuration portal
- NVS save/persisted settings
- v1.37 profile overlay UI change prepared from v1.36

### V27 colour state

The V27 profile pair is the current working colour state after physical LCD testing.

The pair is based on the actual ConsoleBadger boot-green treatment and uses the same 256-entry RGB565 palette/green treatment for:
- `profile_energy`
- `profile_energy_patch`

Physical testing accepted the colour family for now. A somewhat “Matrixy” texture remains, and two horizontal seams around the animated patch region remain recorded as an unresolved visual issue.

### v1.37 profile UI change

Based directly on protected v1.36:
- removes the decorative top `XBOX` label from the Gamer ID screen
- removes GamerScore label/value placeholders
- centres ONLINE and Wi-Fi status together at the bottom
- renders both status indicators/labels white
- leaves Wi-Fi connectivity logic unchanged
- leaves V27 CBP assets, boot, CBP decoder/player, NVS, pinout and SPI settings unchanged

v1.37 still requires Arduino IDE compile/upload and physical LCD verification.

## Product direction

- no dedicated phone app
- local ESP32 setup portal
- Wi-Fi + Xbox Gamertag for first configuration
- settings stored locally in NVS
- no user database
- no persistent MAC customer records
- Xbox profile data via Cloudflare Worker -> OpenXBL service
- 360x360 Xbox-themed profile UI
- 5-minute retention protection

## Evidence and provenance

The Library is the project source of truth for:
- historical versions
- large binary SD assets
- physical LCD video/photo evidence
- encoder investigation and reconstruction records
- checksums and test evidence

The original CBP encoder source has not been recovered. `make_cbp1_reconstructed.py` is a forensic reconstruction and must not be treated as the historical original.

See:
- docs/PRODUCT_ARCHITECTURE_V1.md
- docs/PROFILE_UI_V1.md
- docs/evidence/2026-09-26-v27-colour-lcd-freeze.md
- docs/evidence/2026-09-26-v27-v137-sha256.txt
- firmware/UPCBadger_v1.37_UI_CHANGE.md
- docs/MILESTONE_V1.17_2026-09-24.md
- services/profile-worker/

## Standalone demo

firmware/current/UPCBadger_PROFILE_DEMO_V1_STANDALONE.ino

This deliberately avoids SD/CBP playback so provisioning/profile UI can be tested independently.
