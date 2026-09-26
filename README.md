# UPCBadger / ConsoleBadger

Updated: 2026-09-26

## Active development — v1.61

Latest physically tested firmware: **v1.61**.

V1.61 is the current live-profile integration baseline. It preserves the proven master-animation, SD, TFT, NVS, Wi-Fi and TLS memory-suspension architecture while making the setup portal GamerTag the actual XBL lookup target.

### V1.61 confirmed runtime path

The latest physical test confirmed:

- Wi-Fi connection
- DNS resolution
- TCP 80 and 443 connectivity
- TLS handshake
- configured GamerTag lookup with HTTP 200
- live GamerTag retrieval
- non-zero GamerScore retrieval
- XUID retrieval
- gamerpic HTTP download with HTTP 200
- gamerpic cache written successfully
- target Xbox Live presence request with HTTP 200
- target presence reported ONLINE
- 32 KiB staging buffer restored
- 181-frame master animation restored at 12 FPS

The remaining known issue is **gamerpic rendering**: the image downloads and caches correctly, but the profile renderer currently reports `Gamerpic draw failed`.

### V1.61 identity flow

The GamerTag stored by the setup portal/NVS is the lookup target.

Lookup order:

1. `https://xbl.io/api/v2/search/<gamertag>`
2. `https://api.xbl.io/v2/player/gamertag/<gamertag>`
3. `https://xbl.io/api/v2/friends/search/<gamertag>`

If an XUID is known but presence is absent from the profile response, V1.61 falls back to:

`https://xbl.io/api/v2/<xuid>/presence`

The portal remains authoritative for which account is checked. The live XBL result supplies the displayed live profile values.

### V1.61 display baseline

- NORTH: GamerTag
- EAST: Wi-Fi
- SOUTH: GamerScore
- WEST: Xbox Live presence
- CENTRE: gamerpic area
- EAST/WEST/SOUTH moved approximately 5 mm toward centre

### Refresh model

- First XBL sync: immediately after Wi-Fi is connected and profile memory is available
- Later XBL sync: every 30 minutes
- Wi-Fi indicator: every 30 seconds
- Animation: continuous at the existing 12 FPS cadence

### Protected architecture

V1.61 intentionally does not change:

- master CBP animation
- 181-frame / 12 FPS playback
- TFT geometry and pins
- SD pins and requested 40 MHz operation
- TFT 80 MHz operation
- 32 KiB staging allocation
- TLS memory-suspension ordering
- NVS setup portal
- background Wi-Fi startup

### V1.61 evidence

Firmware source SHA-256:

`48c1c3560d0e32f362e203f6e0094c8aee7b5e1ff8c7392cc39f9ae8c15dab65`

The detailed runtime evidence is stored in `docs/evidence/UPCBadger_EVIDENCE_V1.61_2026-09-26.md`.

## Earlier development history

### V1.50–V1.59 transport/memory work

V1.50 established the OpenXBL transport diagnostic.

V1.51 proved live GamerTag retrieval through the transport path.

V1.52 continued the transport path while profile UI geometry was refined.

V1.54–V1.58 concentrated on profile layout, gamerpic rendering compatibility and LovyanGFX/DataWrapper integration.

V1.59 established the immediate-first XBL check and the 30-minute refresh model, while retaining the TLS memory-suspension strategy.

### V1.49 — TLS memory recovery

V1.48 proved Wi-Fi, DNS and TCP 443 but TLS failed with mbedTLS `-32512 / SSL - Memory allocation failed`.

V1.49 temporarily releases the 32 KiB animation staging buffer and closes the profile CBP file before the TLS handshake, then restores and revalidates animation resources afterward.

### V1.47 — network diagnostic

V1.47 proved DNS, TCP and the exact documented `/v2/account` transport path before player lookup.

### V1.46 — first live profile test

V1.46 introduced controlled live Xbox profile integration using OpenXBL.

## Smooth Engine research branch

A separate engineering investigation is being maintained for eliminating animation stutter during serious background work.

Research areas include:

- ESP32 dual-core task separation
- FreeRTOS task affinity and priorities
- Wi-Fi/TLS isolation
- SPI/DMA
- SD/TFT pipelining
- double buffering
- frame pacing
- decoder isolation
- heap fragmentation
- non-blocking XBL requests

The production V1.61 baseline remains protected while this work is researched separately.

## Secret handling

Real API keys must stay out of GitHub.

Use local-only secrets and safe templates. Never commit a live OpenXBL API key.

## Safety

Before wiring, soldering, or physical modification:

**UNPLUG THE ESP32 FIRST.**
