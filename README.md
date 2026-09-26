# UPCBadger / ConsoleBadger

Updated: 2026-09-26

## Active development — v1.46

Latest physically tested firmware: v1.38.
V1.44 is the protected compile-repair baseline.
V1.45 is abandoned.
V1.46 is the current live-data test build.

### V1.46 purpose

First controlled live Xbox profile integration using OpenXBL and the fixed development test account renaultman172.

The V1.44 master-animation architecture is retained. The current test does not alter the master CBP, loop boundaries, palette, TFT geometry, SD playback path, NVS storage, or product setup flow.

### Live profile fields

V1.46 requests:
- gamertag
- XUID
- gamerscore
- profilePicture URL

OpenXBL currently documents a player-by-gamertag endpoint at https://api.xbl.io/v2/player/gamertag/:gt and X-Authorization API-key authentication, with profile data including gamertag, XUID, gamerscore and profile picture.

### V1.46 test behaviour

- Home Wi-Fi starts using the existing non-blocking product path.
- After the first successful Wi-Fi connection, one live profile lookup is requested.
- The returned gamertag becomes the RAM-only display source.
- NVS is not modified.
- Further profile refreshes use the existing 60-minute scheduler.
- API timeout is limited to a few seconds for the development test.
- No gamerpic image download/rendering is implemented yet.
- Xbox presence/real-time activity is not implemented yet.

OpenXBL currently advertises a free tier of 150 requests/hour.

### Secret handling

Real API keys must stay out of GitHub. OpenXBL's current terms state that API keys are for the user's use and should not be shared.

V1.46 uses:
- Secrets.h — local only
- Secrets.h.example — safe template

### Known compositor issue

The current animation player still redraws the full background and then redraws the profile overlay. V1.46 deliberately establishes real API data first so we can separate changing API data from the display compositor flashing.

### Runtime SD

Required: ConsoleBadger_MASTER.CBP

### Version history

- V1.38 — latest physically proven baseline
- V1.40 — profile UI / loop refinement
- V1.42 — one-master animation architecture
- V1.43 — abandoned wrong-section frame surgery
- V1.44 — compile repair baseline
- V1.45 — abandoned logo-hold experiment
- V1.46 — live OpenXBL profile test

### Safety

Before wiring, soldering, or physical modification:

UNPLUG THE ESP32 FIRST.

## V1.47 — network diagnostic

V1.47 is the current transport test. It proves DNS, TLS/TCP, and the exact documented `/v2/account` request before player lookup. The PC has already returned HTTP 200 with the user's OpenXBL key. The ESP32 previously returned HTTP -1 / connection refused.
