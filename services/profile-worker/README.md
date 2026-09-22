
# UPCBadger Profile Worker — V1

Minimal Cloudflare Worker for the first UPCBadger Xbox profile prototype.

## Purpose

The badge will eventually call:

    GET /profile?gamertag=YOUR_GAMERTAG

The Worker calls OpenXBL and returns only:

- gamertag
- gamerscore
- gamerpic URL
- XUID

The OpenXBL API key stays server-side as a Cloudflare Worker secret.

## Deliberate scope

This first implementation does NOT add:

- database
- user accounts
- MAC registration
- telemetry
- analytics
- RTA/WebSockets
- friends
- achievements
- location or other personal data

Presence/current-game data will be added only after the basic profile request is proven.

## Secret

Set the OpenXBL key as a Worker secret:

    wrangler secret put OPENXBL_API_KEY

Never commit the key to GitHub.

## Current provider endpoint

OpenXBL currently documents:

    GET /v2/player/gamertag/:gt

This Worker uses that endpoint and maps the response into the UPCBadger-specific V1 payload.

## Next test

1. Deploy the Worker.
2. Add the OpenXBL API key as a secret.
3. Test /profile?gamertag=... from a browser/curl.
4. Confirm the returned JSON.
5. Only then connect the ESP32.
