# UPCBadger Controller Detection V1 — Test Sheet

## Goal
Find stable, non-connecting Bluetooth information that can distinguish modern gaming-controller families well enough to select a UPCBadger boot theme.

## Hardware
- Original ESP32 DevKit 1 / DOit-style board
- 360x360 GC9B72 using the locked UPCBadger TFT pinout
- USB cable for flashing/Serial Monitor
- Test controllers one at a time

## Before testing
Power down/unplug the ESP32 before changing any wiring. No wiring changes are required for this firmware.

## Procedure
1. Flash `firmware/experiments/UPCBadger_CONTROLLER_DETECT_V1_STANDALONE.ino`.
2. Open Serial Monitor at 115200 baud.
3. Leave controllers switched off and allow a few scan cycles. Record any baseline devices.
4. Wake exactly one controller using its normal power button.
5. Record every appearance of that controller across several scans.
6. Repeat the same wake-up test at least five times.
7. Repeat with Xbox, DualSense, DualShock 4 and Switch Pro if available.
8. Compare NAME, manufacturer data, service UUIDs, service data, Bluetooth Classic COD, RSSI and any repeated fingerprint material.

## What we are looking for
- Information that appears reliably for the same controller family.
- Information that differs between Xbox / PlayStation / Nintendo.
- Whether the information appears immediately when the controller wakes.
- Whether it remains observable while the controller is connected to its console.
- Whether an address changes between wake-ups.

## Do not assume
A displayed device name alone is not a guaranteed platform identity. A name such as `Wireless Controller` may need additional evidence.

## Success criterion
We only need enough stable information to choose a boot theme. Exact gamepad identification, pairing, control, account access and console communication are out of scope.

## Privacy
This experiment does not connect to controllers and does not persist their Bluetooth addresses. Serial output may show addresses for troubleshooting; do not commit those captures to the repository.

## Next step after successful capture
Build a small local fingerprint table and test automatic platform selection. Only after that should the detection module be considered for integration with the profile/boot application.