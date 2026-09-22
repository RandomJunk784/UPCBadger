# UPCBadger — Xbox Profile API V1

## Intent

Build the first simple cloud-backed Xbox profile branch for UPCBadger prototypes.

The immediate goal is not a production platform. It is to make a family prototype badge able to display a small amount of public Xbox profile information without requiring a laptop or home server to run continuously.

## V1 data

The badge should request only the basic data needed for the first UI:

- Gamertag
- Gamerscore
- Gamerpic
- Online/offline status
- Current game, when available

Add further fields only when there is a concrete UI reason to do so.

## Architecture

ESP32 badge -> HTTPS -> Cloudflare Worker -> OpenXBL -> simplified JSON response -> ESP32

The OpenXBL API key must remain server-side as a secret. It must never be embedded in ESP32 firmware.

The laptop is not part of the runtime architecture.

## Privacy / data handling

V1 should not require:

- User accounts
- Email addresses
- Real names
- Location
- Friends lists
- Messages
- Microsoft passwords
- Persistent customer profiles
- Analytics or telemetry

No user database is required for V1.

A MAC address should not be stored unless a later feature has a concrete need for persistent device identity or rate limiting.

The badge may cache returned profile data locally.

## Request strategy

Keep requests deliberately infrequent and cache-first. The initial implementation should favour a simple refresh interval rather than real-time activity streaming.

## Provider

OpenXBL is the initial provider candidate. Before any public/commercial deployment, verify current provider terms and whether the intended shared-server/product architecture is permitted.

## Hosting

Cloudflare Workers is the initial hosting candidate so the service remains available without a local computer running 24/7.

## First milestone

1. Create the minimal Cloudflare Worker.
2. Keep the OpenXBL credential in a server-side secret.
3. Accept a Gamertag.
4. Retrieve the basic profile fields above.
5. Return a small UPCBadger-specific JSON response.
6. Test from a normal web request before touching ESP32 firmware.
7. Build the first 360x360 profile UI after the data path is proven.

## Deliberate non-goals

Do not build a full account system, database, analytics platform, RTA/WebSocket system, or large API abstraction layer yet.

Start small. Add capability only when the badge actually needs it.
