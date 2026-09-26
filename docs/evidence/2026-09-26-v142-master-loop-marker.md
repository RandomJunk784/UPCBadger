# UPCBadger v1.42 — Master animation loop-marker refinement

Based directly on v1.41.

## Master animation

One file:
`ConsoleBadger_MASTER.CBP`

- 360x360
- 181 frames
- 12 FPS metadata
- boot/transition: frames 0..103
- Gamer ID loop: frames 104..180
- loop duration: 6.4167 s

## v1.42 behaviour

- boot frames 0..103 play once
- at loop marker 104, NVS/settings state is evaluated
- unconfigured first-time units render and hold frame 104 while the existing setup AP/captive portal runs
- configured units continue into the Gamer ID loop
- Gamer ID loops only within 104..180
- manual config-button entry retains the existing setup screen
- existing NVS, saved Gamer ID, Wi-Fi, setup portal, buttons and live overlay remain
- old five-minute black-screen/shift retention remains removed

## Validation

- ZIP integrity PASS
- internal SHA-256 PASS
- source brace balance PASS
- master CBP header PASS
- 181/181 frame CRC PASS
- 181/181 frame decompression PASS
- one active runtime master asset
- no active profile patch/PBG dependency

Outer ZIP SHA-256:
d8c30660bbe7028122faec4393707118b79b6d16a5d7834c0310383a4c645b04

Master CBP SHA-256:
eefce014f5a9537c64f5d8166ec57fc98f53d94ea2a9ce381a756d32e0d55c07

The master CBP is preserved in the Library; GitHub records the architecture, source state and hashes.

Physical verification is pending.
