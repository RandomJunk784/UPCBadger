# UPCBadger / ConsoleBadger

Updated: 2026-09-26

## Active development — v1.43

Latest physically tested firmware: v1.38.
Prepared development builds: v1.39, v1.40, v1.41, v1.42 and v1.43.

**v1.43 is the current circular-energy transition experiment.**

### Master animation

One CBP asset:
`ConsoleBadger_MASTER.CBP`

V1.43:
- 360x360
- 177 frames
- 12 FPS
- boot/transition frames 0..99
- Gamer ID loop frames 100..176
- Gamer ID loop = 6.4167 seconds at 12 FPS
- all 177 frames decode successfully on PC

The master was derived from the user-supplied 12-second Seedance 2.5 master.

### v1.43 transition surgery

PC inspection of the 1–3 second circular-energy build found a visible positional
wobble/reversal before the energy settles at the top of the circular element.

Deleted master frames:
- 31
- 32
- 33
- 34

The later stable continuation at frame 35 is kept.

These correspond to source frames 62, 64, 66 and 68 of the 24 FPS Seedance master.

The edit is payload-preserving:
- 177/177 retained frame payloads are byte-for-byte identical to v1.42
- palette entries are unchanged
- retained frame pixels are unchanged
- only the CBP frame index/header has been rebuilt

### v1.43 firmware

The one-master architecture remains:
- boot section plays once
- configured units continue through the loop marker
- Gamer ID loop repeats continuously
- NVS/setup/Wi-Fi/product layer is retained
- old five-minute black-screen/shift retention remains removed

Runtime SD asset:
`ConsoleBadger_MASTER.CBP`

Previous separate animation assets remain historical/backup material.

### Current UI

- TE removed
- temporary cyan avatar removed
- Gamer ID lower and smaller
- ONLINE moved outward
- Wi-Fi text removed; beacon retained
- central gamerpic well reserved for live Xbox API gamerpic

### Test status

v1.43 is PC-validated but **not yet physically verified**.

Next physical test:
1. Compile/upload v1.43.
2. Check the 1–3 second transition.
3. Confirm the positional wobble is gone.
4. Confirm the energy continues naturally into the next frame.
5. Confirm the Gamer ID loop still behaves correctly.
6. Confirm Lumina colour appearance remains unchanged.

V1.42 is the immediate rollback baseline.

## Evidence

The Library is the project source of truth for source videos, CBP binaries,
physical LCD evidence, engineering handoffs and checksums.

See:
- docs/evidence/2026-09-26-v141-master-animation.md
- docs/evidence/2026-09-26-v142-master-loop-marker.md
- docs/evidence/2026-09-26-v143-transition-surgery.md
- docs/evidence/2026-09-26-v143-checksums.txt

## Encoder recovery

The original historical CBP1 encoder source has not been recovered.
`make_cbp1_reconstructed.py` remains a forensic reconstruction.

## Safety

Before any wiring, soldering or physical modification:

**UNPLUG THE ESP32 FIRST.**
