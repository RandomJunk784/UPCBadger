# UPCBadger / ConsoleBadger

Updated: 2026-09-26

## Active development — v1.41

Latest physically tested firmware: v1.38.
Prepared test firmware: v1.39, v1.40 and v1.41.

**v1.41 is the current master-animation architecture experiment.**

### V1.41 master animation

One CBP asset now contains the entire boot-to-Gamer-ID visual path:

`ConsoleBadger_MASTER.CBP`

Properties:
- 360x360
- 181 frames
- 12 FPS metadata
- boot/transition frames 0..103
- Gamer ID loop frames 104..180
- Gamer ID loop = 77 frames / ~6.42 seconds at 12 FPS
- all 181 frames decode successfully on PC
- stable 256-colour RGB565 palette
- no dithering

The master was built from the user-supplied 12-second Seedance 2.5 master video.

Source selection:
- boot/transition: source frames 0,2,4,...,206
- Gamer ID loop: source frames 208..284 inclusive

The selected Gamer ID endpoints are visually compatible and produce a substantially
smaller loop boundary discontinuity than the original 96 -> 0 wrap.

### V1.41 firmware architecture

v1.41 uses the same master CBP for both startup and Gamer ID.

The product layer remains intact:
- NVS settings
- configured/unconfigured check
- setup SoftAP + captive portal
- saved Gamer ID
- background home Wi-Fi
- profile sync hook
- config/factory-reset button
- live overlay

Playback:
- boot player renders master frames 0..103 once
- configured units enter loop frames 104..180
- Gamer ID background continues moving indefinitely
- old five-minute black/shift retention cycle remains removed

Runtime SD asset for v1.41:
`ConsoleBadger_MASTER.CBP`

The old separate runtime assets are superseded for this experiment:
- `profile_energy.CBP`
- `profile_energy_patch.CBP`
- `profile_bg.pbg`
- `ConsoleBadger_330x350_HYBRID96_192_256_HC9_TEST27B.CBP`

Historical copies remain preserved in the Library.

### UI carried into v1.41

- TE initials removed
- temporary cyan avatar circle removed
- Gamer ID moved lower and smaller
- ONLINE moved outward
- Wi-Fi text removed; white beacon retained
- dark central gamerpic well remains clear for future live Xbox API gamerpic

### Test status

v1.41 is PC-validated but **not yet physically verified**.

Next physical test:
1. compile/upload v1.41
2. verify master boot appearance
3. verify boot -> Gamer ID continuation
4. verify continuous 104..180 loop
5. watch the loop boundary
6. verify live overlay stability

Do not alter the master CBP during the first firmware test.

## Evidence

The Library is the project source of truth for source videos, CBP binaries,
physical LCD evidence, engineering handoffs and checksums.

See:
- docs/evidence/2026-09-26-v28-lumina-single-cbp.md
- docs/evidence/2026-09-26-v139-ui-loop.md
- docs/evidence/2026-09-26-v139-checksums.txt
- docs/evidence/2026-09-26-v141-master-animation.md
- docs/evidence/2026-09-26-v141-checksums.txt

## Encoder recovery

The original historical CBP1 encoder source has not been recovered.
`make_cbp1_reconstructed.py` is a forensic reconstruction and must not be
described as the original encoder.

## Safety

Before any wiring, soldering or physical modification:

**UNPLUG THE ESP32 FIRST.**
