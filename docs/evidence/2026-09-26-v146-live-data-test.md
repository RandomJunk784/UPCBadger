# UPCBadger V1.46 — live data test handoff

Date: 2026-09-26

## Parent
V1.44 compile-repair baseline.

## Purpose
First live Xbox profile test using OpenXBL and test account renaultman172.

## API
Current OpenXBL documentation identifies GET https://api.xbl.io/v2/player/gamertag/:gt.
Authentication uses the X-Authorization API-key header.
Expected profile fields include gamertag, xuid, gamerscore and profilePicture URL.

## Firmware changes
- Added ESP32 HTTPClient + WiFiClientSecure.
- Added local Secrets.h.
- Added Secrets.h.example.
- Added URL encoding for the gamertag path component.
- Added manual JSON extraction to avoid introducing a new JSON-library dependency.
- Added live profile state in RAM.
- Test mode uses renaultman172.
- Successful live gamertag becomes the display text source.
- NVS is not changed.
- Profile image URL is captured and printed but not rendered.
- Xbox presence is not queried yet.
- Existing 60-minute profile scheduler is retained.
- First profile request is triggered once after the first Wi-Fi connection.

## Intentionally unchanged
- ConsoleBadger_MASTER.CBP
- 181-frame master structure
- 12 FPS playback
- boot frames 0..103
- Gamer ID loop 104..180
- TFT pins
- 80 MHz TFT path
- SD pins / SD playback
- NVS settings
- setup AP/captive portal
- old retention routine remains removed

## Security note
The development HTTP client uses WiFiClientSecure::setInsecure() to avoid certificate-management complexity during this first hardware test. This is not acceptable as a finished production security design.
Real API keys must never be committed.

## Validation
- source brace balance: PASS
- required API includes: PASS
- test account present: PASS
- OpenXBL endpoint present: PASS
- X-Authorization header present: PASS
- live gamertag state present: PASS
- gamerpic URL state present: PASS
- NVS mutation not introduced by live sync: PASS
- ZIP integrity: PASS

Physical Arduino compile/LCD verification is pending.

## Next observation
Capture Serial Monitor output showing HTTP status 200, returned gamertag, XUID, gamerscore and gamerpic URL.
Do not judge the final compositor yet. This build is deliberately a live-data integration experiment.

## Rollback
V1.44.