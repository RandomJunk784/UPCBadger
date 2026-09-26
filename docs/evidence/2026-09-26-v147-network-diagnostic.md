# V1.47 Network Diagnostic

The PC has already returned HTTP 200 from OpenXBL /v2/account using the user's API key. V1.46 on ESP32 instead returned HTTP -1 / connection refused.

V1.47 therefore isolates transport in three stages:
- DNS resolution
- TCP/TLS connection to api.xbl.io:443
- exact GET /v2/account request with X-Authorization

No CBP, animation, UI, NVS, or product-layer changes are made.

Interpretation:
- DNS fail: local DNS/network issue
- TLS connect fail after DNS: ESP32 outbound HTTPS/TLS path issue
- TLS passes but HTTP -1: HTTPClient/request-layer issue
- HTTP 401/403: key/authentication issue
- HTTP 200: transport proven; proceed to renaultman172 player lookup

V1.47 source SHA-256: 8ecff31d48cb1e00998e86daacb7ad1036672111e83dbffa908401dff64e727c
V1.47 package SHA-256: 12c6f20b4a17a76906803e25d2e2731bb294e375a2abc7e74b0f19eb4b6df5d3
