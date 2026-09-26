# UPCBadger / ConsoleBadger

Updated: 2026-09-26

## Active development — v1.44

Latest physically tested firmware: v1.38.
Prepared development builds: v1.39, v1.40, v1.41, v1.42, v1.43 and v1.44.

**v1.44 is a compile-only repair of the v1.42 master-animation architecture.**

### V1.42 master architecture retained

One CBP asset:
`ConsoleBadger_MASTER.CBP`

- 360x360
- 181 frames
- 12 FPS metadata
- boot/transition frames 0..103
- Gamer ID loop frames 104..180
- Gamer ID loop = 6.4167 seconds at 12 FPS
- stable shared 256-colour RGB565 palette
- no dithering

### V1.44 change

The user's Arduino IDE reported:
`startConfigMode was not declared in this scope`

V1.44 fixes this without changing the product or animation architecture:
- early forward declaration added before the early factory-reset/config-button callers
- early calls made explicit with `startConfigMode(false)`
- late duplicate prototype removed
- version banner updated to v1.44

No CBP asset bytes were changed.

The canonical V1.42 master SHA remains:
`eefce014f5a9537c64f5d8166ec57fc98f53d94ea2a9ce381a756d32e0d55c07`

### Product layer retained

- NVS/settings
- saved Gamer ID
- setup SoftAP/captive portal
- home Wi-Fi
- profile sync hook
- config/factory reset button
- live overlay
- loop-marker behaviour
- old five-minute black-screen/shift retention remains removed

### Runtime SD asset

Required:
`ConsoleBadger_MASTER.CBP`

### Status

V1.44 is PC/static validated.

Arduino IDE 2.3.10 / ESP32 3.3.11 compilation must be performed in the user's Windows environment.

Physical LCD verification is pending.

V1.43 remains a failed, abandoned transition-surgery experiment and must not be used.

V1.42 remains the immediate animation/architecture baseline.

## Evidence

See:
- docs/evidence/2026-09-26-v142-master-loop-marker.md
- docs/evidence/2026-09-26-v143-transition-surgery.md
- docs/evidence/2026-09-26-v144-compile-repair.md
- docs/evidence/2026-09-26-v144-checksums.txt

## Encoder recovery

The original historical CBP1 encoder source has not been recovered.
`make_cbp1_reconstructed.py` remains a forensic reconstruction.

## Safety

Before any wiring, soldering, or physical modification:

**UNPLUG THE ESP32 FIRST.**
