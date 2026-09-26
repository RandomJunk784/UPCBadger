# UPCBadger / ConsoleBadger

Updated: 2026-09-26

## Active development — v1.42

Latest physically tested firmware: v1.38.
Prepared development builds: v1.39, v1.40, v1.41, v1.42 and v1.43.

**v1.42 is the current active test baseline.**

### Master animation

One CBP asset:
`ConsoleBadger_MASTER.CBP`

- 360x360
- 181 frames
- 12 FPS metadata
- boot/transition frames 0..103
- Gamer ID loop frames 104..180
- Gamer ID loop = 6.4167 seconds at 12 FPS
- all 181 frames decode successfully on PC
- stable shared 256-colour RGB565 palette
- no dithering

The master is derived from the user-supplied 12-second Seedance 2.5 master.

Source selection:
- boot/transition: source frames 0,2,4,...,206
- Gamer ID loop: source frames 208..284

The loop endpoints were selected for visual compatibility.

### v1.42 playback architecture

The same master CBP is used for startup and Gamer ID.

- boot frames 0..103 play once
- frame 104 is the Gamer ID loop marker
- configured units continue from frame 104
- Gamer ID loops only through frames 104..180
- first-time/unconfigured units pause on frame 104 while the existing setup AP/captive portal operates
- manual config-button entry retains the normal setup screen
- old five-minute black-screen/shift retention remains removed

The existing product layer remains:
- NVS/settings
- saved Gamer ID
- setup SoftAP/captive portal
- home Wi-Fi
- profile sync hook
- config/factory-reset button
- live overlay

### v1.43 — abandoned experiment

v1.43 attempted to surgically remove a suspected wobble from the wrong section
of the derived/master frame sequence.

The user rejected the result and explicitly abandoned v1.43 to avoid entering
a loop-editing mess.

**v1.43 must not be treated as current or as evidence that the intended
1–3 second source-video wobble was solved.**

V1.42 is the rollback point.

The v1.43 package, CBP and evidence remain preserved as historical failed work.

### Runtime SD asset for v1.42

Required:
`ConsoleBadger_MASTER.CBP`

Previous separate runtime assets and the v1.43 experimental master remain
historical/backup material.

### Current UI inherited by v1.42

- TE initials removed
- temporary cyan avatar removed
- Gamer ID lower and smaller
- ONLINE moved outward
- Wi-Fi text removed; beacon retained
- dark central gamerpic well reserved for future live Xbox API gamerpic

### Test status

v1.42 is PC-validated but **not yet physically verified**.

Next physical test:
1. Compile/upload v1.42.
2. Verify boot and transition.
3. Verify first-time setup pauses at the loop marker.
4. Verify configured units continue into the Gamer ID loop.
5. Verify the 6.42-second Gamer ID loop is visually continuous.
6. Verify live overlay stability.

Do not alter the master CBP during the first physical test.

## Evidence

The Library is the project source of truth for source videos, CBP binaries,
physical LCD evidence, engineering handoffs and checksums.

See:
- docs/evidence/2026-09-26-v141-master-animation.md
- docs/evidence/2026-09-26-v142-master-loop-marker.md
- docs/evidence/2026-09-26-v143-transition-surgery.md
- docs/evidence/2026-09-26-v143-checksums.txt
- docs/evidence/2026-09-26-v142-rollback.md

## Encoder recovery

The original historical CBP1 encoder source has not been recovered.
`make_cbp1_reconstructed.py` is a forensic reconstruction and must not be
described as the original encoder.

## Safety

Before any wiring, soldering or physical modification:

**UNPLUG THE ESP32 FIRST.**
