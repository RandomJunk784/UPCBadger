# UPCBadger / ConsoleBadger — 2026-09-26 V27 Colour / LCD Freeze

## Current working colour state

V27 is the current working CBP colour state after physical LCD testing.

The complete LCD sequence supplied on 2026-09-26 is retained as physical evidence.

Working CBP pair:
- profile_energy_V27_BOOTGREEN_EXACT.CBP
- profile_energy_patch_V27_BOOTGREEN_EXACT.CBP

The pair uses the same 256-entry RGB565 palette / green treatment.

## Physical result

The LCD now has the intended Xbox/ConsoleBadger green family and reads as one coherent visual system.

User accepted V27 as the working colour state for now. The user described the remaining texture as somewhat "Matrixy", attributable to the current palette/quantisation approach.

Two horizontal seams/bands remain visible around the animated patch region. Their positions are consistent with the 360x140 patch boundary at Y=110. This remains a separate patch/background boundary investigation and is intentionally not part of the v1.37 UI change.

## Encoder findings

- Original CBP1 encoder source was not recovered.
- make_cbp1_reconstructed.py is a forensic reconstruction, not the historical encoder.
- CBP1 uses indexed 8-bit frame data, a 256-entry RGB565 palette and LZ4-compressed blocks.
- Profile palette bytes are stored in display wire order / MSB-first.
- The earlier psychedelic patch failure was traced to RGB565 palette byte order and corrected without changing the firmware playback path.
- The exact historical patch-generation/precomposition stage remains unresolved.
- Historical evidence indicates the patch was precomposited against the profile background and feathered at its outer region.

## ConsoleBadger boot-green reference

The current procedural boot source uses explicit green values including:
- (80,255,60)
- (40,255,40)
- (30,150,30)
- (20,180..230,20)
- (100,255,100)

These are the relevant ConsoleBadger boot-green reference values for future colour work.

## v1.37 UI-only change

Based directly on protected v1.36.

Removed:
- decorative XBOX text at the top of the Gamer ID screen
- GAMERSCORE label
- gamerscore placeholder value

Added/changed:
- one centred bottom status row
- ONLINE indicator + ONLINE text
- Wi-Fi indicator + Wi-Fi text
- both status labels/indicators are white
- live home-Wi-Fi connection logic is unchanged

Preserved:
- V27 CBP colour assets
- boot animation
- CBP decoder/player
- NVS/configuration flow
- profile background and animated patch architecture
- TFT/SD pinout and SPI settings

## v1.37 test status

PC static checks:
- overlay no longer contains XBOX or GAMERSCORE text
- ONLINE and Wi-Fi labels are present
- status colour is white
- braces balanced
- ZIP integrity checked

Not yet physically verified:
- Arduino IDE compile/upload
- physical LCD result

