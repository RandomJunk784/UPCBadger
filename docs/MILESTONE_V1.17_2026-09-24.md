# UPCBadger — Milestone v1.17
## 2026-09-24

STATUS: PROTECTED WORKING BASE

v1.17 is the first confirmed end-to-end ConsoleBadger product integration on the project hardware.

Verified flow:
SD initialisation → CBP index load → Xbox boot animation → Gamer ID/profile → background home Wi-Fi → configuration portal → Wi-Fi scan → NVS save → reboot → persisted Gamer ID restored → Wi-Fi reconnect.

Key v1.14 evidence:
- Heap before SD: 206644
- SD.begin(): PASS
- CBP index: PASS
- 96 KiB staging allocation after SD: FAIL

Key v1.16 evidence:
- Heap before SD: 206644
- Largest free block before SD: 110580
- SD.begin(): PASS
- CBP index: PASS
- 32 KiB staging allocation: PASS
- Frame 0 failed with invalid CBP block

v1.17 change:
- Restored the indexed frame seek before playFramePipelined(), matching the proven Library playback paths.

v1.17 result:
- Xbox animation completed.
- Gamer ID/profile screen activated.
- Background Wi-Fi connected.
- Configuration SoftAP started on 192.168.4.1.
- Captive DNS and HTTP server worked.
- Client connected.
- Wi-Fi scan returned 9 networks.
- /save accepted Wi-Fi credentials and Gamer ID.
- NVS save completed.
- Automatic restart occurred.
- Next boot restored the saved Gamer ID and reconnected to Wi-Fi.

Non-blocking warning recorded:
wifi_init_default: netstack cb reg failed with 12308
The AP then started successfully and HTTP/scanning worked, so this is recorded as a warning rather than a demonstrated failure.

IMPORTANT:
The tested firmware still prints v1.16 in its runtime banner. Do not alter the working firmware merely to correct that cosmetic text.

NEXT TARGET:
Improve the static Gamer ID/profile screen visually. Preserve the working playback, NVS, configuration and Wi-Fi flow. Live Xbox data retrieval comes after the static UI is satisfactory.

VERSION:
v1.17 protected. Next new change is v1.18.

ZIP:
Arduino test packages are ZIP-by-default, complete, flat, with no enclosing or nested folder. Double check before delivery.

SAFETY:
Unplug the ESP32 before wiring changes or soldering.