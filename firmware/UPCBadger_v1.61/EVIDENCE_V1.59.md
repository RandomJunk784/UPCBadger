# V1.59 Runtime Evidence

Observed successful account transport:
- Wi-Fi connected
- DNS OK
- TCP 80 OK
- TCP 443 OK
- TLS CONNECT OK
- `/v2/account` HTTP 200
- Live GamerTag returned by the API
- 32 KiB staging buffer restored
- Profile playback restored

Observed V1.59 limitations:
- The displayed target continued to follow the API-key account because `/v2/account` was queried instead of the portal GamerTag.
- GamerScore reported 0 in the test run.
- Gamerpic request returned HTTPClient transport error `-1`.
- Presence request returned HTTPClient transport error `-1`.

V1.61 addresses the identity-flow error and adds conservative handling for the remaining failure states.