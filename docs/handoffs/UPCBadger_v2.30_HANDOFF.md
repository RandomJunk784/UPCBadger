# UPCBadger v2.30 — 5-Minute Xbox Live Presence Poll

## BASE
v2.28 — physically working baseline.

## PURPOSE
Make the left XBL status bar reflect a fresh target-player presence query every 5 minutes, while keeping the 20-minute profile refresh for GamerTag/GamerScore/gamerpic.

## CHANGE
- Added a 5-minute target-XUID presence service using `https://xbl.io/api/v2/{XUID}/presence`.
- 20-minute profile refresh remains unchanged.
- 60-second Wi-Fi state check remains unchanged.
- 12 FPS animation/static compositor remains unchanged.
- Presence failure retains the last known state.
- No new large persistent framebuffer/buffer was added.

OpenXBL currently documents presence polling as a supported approach and advertises 150 requests/hour on its free tier. A five-minute poll is 12 presence requests/hour before the separate profile traffic. citeturn158972search0turn354719search3

## PHYSICAL STATUS
v2.28 remains the latest physically verified baseline. v2.30 is not yet physically flashed.

Before wiring/soldering: **UNPLUG THE ESP32 FIRST.**
