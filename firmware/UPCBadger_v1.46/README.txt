UPCBadger v1.46 — LIVE DATA TEST

Parent:
  V1.44 compile-repair baseline.

Purpose:
  First controlled live Xbox profile test using OpenXBL.

Test account:
  renaultman172

Live fields captured:
  gamertag
  XUID
  gamerscore
  profilePicture URL

Display:
  Live gamertag replaces the RAM-only profile display source after a
  successful API response. NVS is NOT changed.

Current API behaviour:
  - Home Wi-Fi connection remains the existing non-blocking startup path.
  - On the first Wi-Fi-connected loop, one OpenXBL profile request is made.
  - Subsequent profile requests use the existing 60-minute scheduler.
  - API timeout is 3.5 seconds.
  - TLS certificate verification is intentionally disabled for this
    development test only (WiFiClientSecure::setInsecure()). Do not treat this
    as production security.
  - No gamerpic download/rendering is implemented yet.
  - Xbox online presence is not yet queried.

API source:
  Current OpenXBL documentation identifies:
  GET /v2/player/gamertag/:gt
  with X-Authorization API-key authentication.

Secret handling:
  Put the real key only in local Secrets.h.
  Never commit the real key to GitHub.
  Secrets.h.example is safe to publish.

Required local libraries:
  Arduino / ESP32 WiFi
  HTTPClient (ESP32 core)
  WiFiClientSecure (ESP32 core)
  LovyanGFX 1.2.29
  existing UPCBadger dependencies

Expected Serial evidence:
  [API] OpenXBL live profile sync requested.
  [API] Lookup: renaultman172
  [API] HTTP status: 200
  [API] LIVE PROFILE RECEIVED.
  [API] Gamertag: ...
  [API] XUID: ...
  [API] Gamerscore: ...
  [API] Gamerpic: https://...

Important:
  The current animation compositor is NOT fixed in V1.46.
  The purpose of this test is to establish real API data in the existing
  runtime before implementing the stable-layer compositor.