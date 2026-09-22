# UPCBadger Profile UI V1

## Purpose
Prove the complete user-facing product shell before connecting live Xbox data.

## Standalone demo firmware
firmware/current/UPCBadger_PROFILE_DEMO_V1_STANDALONE.ino

This build intentionally does not use the SD card or CBP boot asset. It isolates the new Wi-Fi/provisioning/profile layer so it cannot damage the proven animation path.

## User flow
First boot: power on -> UPCBadger creates temporary setup Wi-Fi -> phone connects -> browser opens local setup page -> enter Wi-Fi SSID + password + Xbox Gamertag -> Save & Connect -> badge restarts and joins home Wi-Fi -> themed boot -> profile screen.

Later setup: hold rear CONFIG button for about 3 seconds -> setup mode.

Factory re-provisioning: hold the same button for 15 seconds, then release -> clear stored Wi-Fi/Gamertag settings -> enter setup mode for a new owner/Gamertag. The 15-second action is evaluated on button release so the 3-second setup action cannot trigger during a factory reset hold.

## Physical button
Reserve GPIO32.
GPIO32 -> momentary button -> GND.
Firmware uses the internal pull-up, so no external resistor is required.

## Profile screen
V1 includes UPCBadger header, PROFILE title, circular gamer identity area, Gamertag, Gamerscore, Wi-Fi state and a clearly labelled PROFILE DEMO status.

## Retention protection
After five minutes the display fades down, holds black briefly, restores, and moves content by 2 pixels on alternating cycles.

## Live data boundary
ESP32 -> HTTPS -> UPCBadger Worker -> OpenXBL.
The ESP32 must never contain the OpenXBL secret.

## Next milestone
Deploy and test the Worker independently, then replace the demo profile source with the actual Worker JSON response.