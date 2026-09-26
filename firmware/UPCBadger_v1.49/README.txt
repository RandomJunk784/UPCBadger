UPCBadger v1.49 — TLS MEMORY RECOVERY TEST

Parent: V1.48.

V1.48 physical evidence:
  Wi-Fi connected.
  DNS resolved api.xbl.io.
  Raw TCP 443 connected.
  TLS handshake failed immediately with mbedTLS error -32512:
  SSL - Memory allocation failed.

V1.49 purpose:
  Determine whether the Badger animation/storage runtime is starving the
  contiguous heap required by WiFiClientSecure / mbedTLS.

Before TLS:
  - print total free heap and largest free block
  - close the profile CBP file
  - release the 32 KiB heap staging buffer

After the TLS/API attempt:
  - restore the 32 KiB staging buffer
  - reopen and revalidate the master CBP handle

Transport sequence:
  DNS -> raw TCP 443 -> TLS -> /v2/account -> restore animation resources

This build does NOT modify:
  - ConsoleBadger_MASTER.CBP
  - palette
  - 181-frame structure
  - 12 FPS timing
  - TFT geometry/pins
  - NVS settings
  - UI layout

Expected useful output:
  [MEM] Before TLS suspension free=... largest=...
  [MEM] After TLS suspension free=... largest=...
  [NET] TCP 443 OK.
  [NET] TLS CONNECT OK.
  [API] HTTP status: 200

Interpretation:
  - largest block rises and TLS succeeds: animation runtime heap pressure was
    the blocker.
  - largest block rises but TLS still returns -32512: mbedTLS/core TLS
    footprint requires a further investigation.
  - largest block barely changes: the staging buffer/file handle was not the
    main pressure source.

Security:
  The transport test uses WiFiClientSecure::setInsecure(). This is for the
  development diagnostic only, not production security.
  Real Secrets.h must remain local and must never be committed.

SAFETY:
  UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.