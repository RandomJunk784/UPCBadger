# UPCBadger Profile Worker — V1

Prototype cloud endpoint for the UPCBadger Xbox profile layer.

Endpoint: GET /profile?gamertag=YOUR_GAMERTAG

The Worker checks a 15-minute edge cache, calls OpenXBL on a cache miss, and returns only gamertag, gamerscore, gamerpic and XUID.

No UPCBadger database is required.

## Secret

The OpenXBL API key is a Cloudflare Worker secret. Never put it in firmware or GitHub.

Set it with:

npx wrangler secret put OPENXBL_API_KEY

Because wrangler.toml declares OPENXBL_API_KEY as required, configure the secret before the first deployment.

## Deployment

From this directory:

npx wrangler login
npx wrangler secret put OPENXBL_API_KEY
npx wrangler deploy

Cloudflare documents Worker secrets as encrypted bindings intended for API keys and tokens.

The deployed Worker URL can later be inserted into the live ESP32 profile client.

## V1 scope

Do not add accounts, MAC registration, telemetry, analytics, RTA/WebSockets, or a database yet.

Presence/current game and direct gamerpic rendering remain follow-on work after the basic profile request is proven.