# UPCBadger V1.61 — Runtime Evidence
Date: 2026-09-26

## Confirmed
The physical V1.61 test successfully exercised the configured GamerTag lookup path against OpenXBL.

Serial evidence confirmed:
- Wi-Fi connected
- DNS lookup succeeded
- TCP 80 succeeded
- TCP 443 succeeded
- TLS handshake succeeded
- configured target GamerTag lookup returned HTTP 200
- live GamerTag returned
- GamerScore returned as a non-zero value
- XUID returned
- gamerpic reported AVAILABLE
- gamerpic download returned HTTP 200
- gamerpic cached successfully at 79,840 bytes in the observed run
- target presence request returned HTTP 200
- Xbox Live presence returned ONLINE
- V1.61 transport path PASS
- 32 KiB staging buffer restored
- profile playback restored

## Remaining failure
The cached gamerpic did not render:

`[PROFILE] Gamerpic draw failed.`

This is now a display/decode/rendering problem, not an XBL transport or image-download problem.

## Important boundary
The V1.61 test proves that the portal-configured GamerTag can be used to retrieve another target profile rather than silently querying the API-key owner's account.

## Source hash
`UPCBadger_v1.61.ino` SHA-256:

`48c1c3560d0e32f362e203f6e0094c8aee7b5e1ff8c7392cc39f9ae8c15dab65`

No API key or XUID is recorded in this evidence file.

**UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.**
