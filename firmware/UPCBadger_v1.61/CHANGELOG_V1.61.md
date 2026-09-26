# UPCBadger V1.61 — Changelog

## Controlled change
V1.61 is based directly on the physically tested V1.59 source.

### Identity flow
The GamerTag stored by the setup portal/NVS is now the actual XBL lookup target. The firmware no longer uses `/v2/account` as the displayed-profile source.

Lookup order:
1. `https://xbl.io/api/v2/search/<gamertag>`
2. `https://api.xbl.io/v2/player/gamertag/<gamertag>`
3. `https://xbl.io/api/v2/friends/search/<gamertag>`

The response is parsed for GamerTag, GamerScore, XUID, gamerpic URL and presence state. If presence state is absent but XUID is known, V1.61 falls back to `https://xbl.io/api/v2/<xuid>/presence`.

### Display baseline
- NORTH: GamerTag unchanged
- EAST: Wi-Fi moved inward by approximately 5 mm
- SOUTH: GamerScore moved inward/up by approximately 5 mm
- WEST: Xbox Live presence moved inward by approximately 5 mm
- CENTRE: gamerpic retained in the 72 px central area

### Refresh model
- First XBL sync: immediately after Wi-Fi is connected and profile memory is available
- Later XBL sync: every 30 minutes
- Wi-Fi indicator: every 30 seconds
- Animation: continuous at the existing 12 FPS cadence

### Safety / failure behaviour
Presence has three states: ONLINE, OFFLINE, UNKNOWN. Transport failure cannot create a false OFFLINE result.

Gamerpic download follows redirects and only re-fetches when the live URL changes or the SD cache is missing.

No changes were intentionally made to the proven boot animation, CBP decoder, SD pins, TFT pins, staging buffer size, TLS memory-suspension ordering, NVS portal, or Wi-Fi connection mechanism.

**UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.**
