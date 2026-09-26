UPCBadger v1.47 — NETWORK DIAGNOSTIC

Parent: V1.46.

Purpose:
Prove the ESP32 -> api.xbl.io transport before returning to player lookup.

Sequence:
1. Wi-Fi connected check
2. DNS resolve api.xbl.io
3. TLS/TCP connection probe to api.xbl.io:443
4. GET /v2/account using X-Authorization
5. HTTP status + short response preview

Expected success:
[NET] DNS OK: ...
[NET] TCP/TLS CONNECT OK.
[API] HTTP status: 200
[API] ACCOUNT REQUEST SUCCESS.
[NET] V1.47 transport path PASS.

A 200 result proves the ESP32 transport and key are working. We then return to
renaultman172 player lookup in the next build.

Unchanged:
- master CBP
- 12 FPS playback
- TFT/SD pins and clocks
- NVS/product flow
- profile layout

Security:
Keep real Secrets.h local. The development TLS probe uses setInsecure();
this is not the final production security design.

SAFETY: UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.
