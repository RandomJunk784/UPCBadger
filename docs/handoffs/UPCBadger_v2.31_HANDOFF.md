# UPCBadger v2.31 — Immediate Wi-Fi Status

Base: v2.30.

The Wi-Fi status bar now lights as soon as the local STA reports `WL_CONNECTED`, and goes white immediately when disconnected. The existing 60-second status service remains as periodic verification.

Unchanged:
- XBL presence poll: 5 minutes
- GamerTag/GamerScore/gamerpic profile refresh: 20 minutes
- Animation: 12 FPS
- static compositor
- Wi-Fi retry: 20 seconds
- gamerpic / SD / CBP / TLS / memory path
- display geometry and pinout

v2.30 remains the last physical test revision with the new 5-minute presence behaviour. v2.31 is not yet physically verified.

UNPLUG THE ESP32 BEFORE WIRING OR SOLDERING.
