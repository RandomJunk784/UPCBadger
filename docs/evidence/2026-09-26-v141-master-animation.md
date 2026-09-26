# UPCBadger v1.41 — Master Animation Experiment

Prepared 2026-09-26.

One full-screen CBP1 master asset now supplies both the ConsoleBadger boot and
the Gamer ID loop.

## Master asset

Library source-of-truth:
`/UPCBadger/06_ASSETS_MEDIA/BOOT/ConsoleBadger_MASTER.CBP`

Properties:
- 360x360
- 181 frames
- 12 FPS metadata
- 181 frames decode successfully on PC
- 8 CBP blocks per frame
- single stable 256-entry RGB565 palette

Logical regions:
- master frames 0..103 = boot/transition
- master frames 104..180 = Gamer ID loop
- loop duration = 77 / 12 = 6.4167 seconds

The master was derived from:
`lumina-12seconds-master.mp4` (640x640, 24 FPS, 289 frames).

Frame construction:
- boot/transition: source frames 0,2,4,...,206
- Gamer ID loop: source frames 208..284 inclusive

The loop endpoints correspond to source frames 208 and 284 and were selected
because they have closely compatible energy geometry.

## Why this architecture

The earlier profile used separate background + patch CBPs. V1.41 removes that
runtime split.

ONE master file:
boot -> transition -> Gamer ID loop

The firmware treats the master as two logical regions:
- boot section plays once
- configured device then loops only the Gamer ID section

This preserves the existing product layer instead of replacing NVS/setup/Wi-Fi.

## Product layer retained

- NVS settings
- configured/unconfigured check
- setup SoftAP/captive portal
- saved Gamer ID
- background home Wi-Fi
- config/factory-reset button
- profile synchronisation hook
- live profile overlay

The old five-minute black-screen/shift retention cycle remains removed.

## V1.41 UI retained

- TE initials removed
- temporary cyan avatar circle removed
- Gamer ID smaller and lower
- ONLINE shifted outward
- Wi-Fi text removed; beacon retained
- central dark gamerpic well reserved for live API gamerpic

## PC validation

- CBP1 header validation: PASS
- 181 frame CRCs: PASS
- 181 frame payload decompressions: PASS
- ZIP integrity: PASS
- internal SHA-256: PASS
- source brace balance: PASS
- active runtime profile asset references point to master CBP
- old profile patch/PBG assets are not active runtime dependencies

The master palette is a new RGB565-aware shared palette built from the combined
boot + Gamer ID material. It does not use the old V27 palette.

## Physical status

V1.41 has not yet been compiled/flashed or physically tested.

The physical V1.38 result remains the last proven hardware state.

## Next test

Compile/upload v1.41 with only:
`ConsoleBadger_MASTER.CBP`

Expected:
- ConsoleBadger boot plays once
- at frame 104 the product state continues into Gamer ID when configured
- Gamer ID loops frames 104..180
- no return to the boot animation during normal looping
- live overlay remains stable

Before wiring/soldering:
**UNPLUG THE ESP32 FIRST.**
