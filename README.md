# UPCBadger / ConsoleBadger

Updated: 2026-09-26

## Active development — v1.45

Latest physically tested firmware: v1.38.
Prepared development builds: v1.39, v1.40, v1.41, v1.42, v1.43, v1.44 and v1.45.

**v1.45 is the current animation test build.**

### Master animation

One CBP asset:
`ConsoleBadger_MASTER.CBP`

- 360x360
- 181 frames
- 12 FPS
- boot/transition 0..103
- Gamer ID loop 104..180
- shared 256-colour RGB565 palette

### v1.45 logo-boundary correction

The Seedance master contains an oversized ConsoleBadger wordmark in the early
boot/transition section.

Direct 12 FPS inspection identified:
- M11 / 0.92s = last visually safe frame
- M12..M21 / 1.00..1.75s = clipped wordmark excursion
- M22 / 1.83s = first safe continuation

V1.45 retains exactly 181 frames:
- 0..11 unchanged
- 12..21 = exact repeats of frame 11
- 22..180 unchanged

This creates a deliberate 10-frame hold (0.8333s) and then resumes when the
wordmark is safely back inside the composition.

No scaling, recolouring, palette modification, or pixel transformation was used.
Every output frame payload is an exact payload selected from the canonical V1.42
master.

V1.45 master SHA:
`5fb956ae6d0ad659a2fa08af54a3efef85b1ee2b1a36806ecaa6aeaa7b8ea5a0`

### Firmware

V1.45 firmware is functionally identical to the V1.44 compile-repair build.
Only the version/changelog label changed.

The V1.42/V1.44 product architecture remains:
- one master CBP
- loop marker at frame 104
- NVS/settings
- setup SoftAP/captive portal
- saved Gamer ID
- home Wi-Fi
- live overlay
- old five-minute black-screen/shift retention removed

### Runtime SD

Required:
`ConsoleBadger_MASTER.CBP`

### Previous experiments

V1.43 is an abandoned wrong-section transition-surgery experiment.
V1.44 is the clean compile repair baseline.
V1.42 remains the immediate animation rollback baseline.

### Status

V1.45 has passed PC validation and package integrity checks.
**Physical LCD verification is pending.**

Next test:
1. Compile/upload V1.45.
2. Watch 0.9–2.0 seconds closely.
3. Confirm the wordmark stays inside the safe display boundary.
4. Confirm the brief hold looks intentional.
5. Confirm M22 continuation is clean.
6. Confirm Gamer ID loop is unchanged.

## Evidence

See:
- docs/evidence/2026-09-26-v145-logo-hold.md
- docs/evidence/2026-09-26-v144-compile-repair.md
- docs/evidence/2026-09-26-v142-master-loop-marker.md

## Safety

Before wiring, soldering, or physical modification:

**UNPLUG THE ESP32 FIRST.**
