# UPCBadger Profile Worker — V1

Prototype cloud endpoint for the UPCBadger Xbox profile layer.

Endpoint: GET /profile?gamertag=YOUR_GAMERTAG

The Worker checks a 15-minute edge cache, calls OpenXBL on a cache miss, and returns only gamertag, gamerscore, gamerpic and XUID.

No UPCBadger database is required.

Secret:
npx wrangler secret put OPENXBL_API_KEY

Never commit the key to GitHub. Cloudflare documents Worker secrets as encrypted bindings intended for API keys and tokens.

Deployment:
npx wrangler login
npx wrangler deploy
npx wrangler secret put OPENXBL_API_KEY

The deployed Worker URL can later be inserted into the live ESP32 profile client.

V1 deliberately excludes accounts, MAC registration, telemetry, analytics, RTA/WebSockets and a database. Presence/current game and direct gamerpic rendering remain follow-on work.