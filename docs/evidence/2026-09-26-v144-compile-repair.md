# UPCBadger v1.44 — V1.42 compile repair

Date: 2026-09-26

## Reason for version bump

V1.42 failed to compile in the user's Arduino IDE because calls to
`startConfigMode()` occurred before the compiler had a usable declaration in
the actual sketch being compiled.

V1.44 is a compile-only repair of V1.42.

## Exact changes

- Added an early forward declaration:
  `static void startConfigMode(bool preserveDisplay);`
- Removed the late duplicate forward declaration.
- Made every call explicit:
  `startConfigMode(false);` or `startConfigMode(true);`
- Updated firmware banner/version from v1.42 to v1.44.

## Intentionally unchanged

- ConsoleBadger_MASTER.CBP bytes
- V1.42 master frame order
- 181 frames / 360x360 / 12 FPS
- boot frames 0..103
- Gamer ID loop frames 104..180
- NVS/settings logic
- setup SoftAP/captive portal
- saved Gamer ID
- background Wi-Fi
- config/factory reset handling
- V1.40 UI arrangement
- removal of the old retention cycle

## Static validation

- declaration appears before every call: PASS
- only one forward declaration: PASS
- definition present exactly once: PASS
- calls all supply the bool argument: PASS
- source brace balance: PASS
- master CBP SHA-256 matches canonical V1.42: PASS
- ZIP integrity: PASS

Arduino IDE 2.3.10 / ESP32 3.3.11 compilation remains to be performed by the
user's Windows environment.

V1.44 does not claim physical verification.

## Package

`UPCBadger_v1.44.zip`

Outer SHA-256:
8393918cd98f0487b0e1c65c695cb4dfd3409140691947f29b92e0c7712d62e5

Master CBP SHA-256:
eefce014f5a9537c64f5d8166ec57fc98f53d94ea2a9ce381a756d32e0d55c07

Safety:
Before wiring, soldering or physical modification:
**UNPLUG THE ESP32 FIRST.**
