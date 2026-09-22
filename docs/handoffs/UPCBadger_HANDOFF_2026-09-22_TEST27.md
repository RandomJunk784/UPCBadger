# UPCBadger Handoff — 2026-09-22 — TEST 27

## Current state

The fastest confirmed 330x350 playback base is the TEST 19/21 path.

Verified diagnostic baseline:
- 330x350 active window
- X=15, Y=5
- GC9B72 360x360
- TFT SPI 80 MHz
- SD SPI request 40 MHz
- 2 x 32 KiB DMA buffers
- 96 KiB staging
- LZ4 HC9
- palette-based CBP
- runtime RGB565 byte swap OFF

TEST 21 diagnostic runs established roughly 13.37–14.06 FPS depending on the encoded asset.

## Locked baselines

### TEST 18
Locked 324x360 performance baseline.
Latest measured run: 13.545 FPS.

### 330x350 fast base
TEST 19 measured 13.363 FPS.
TEST 21 diagnostics reproduced essentially the same playback path and performance.

### Palette experiments
128 active colours: about 13.94 FPS across two runs.
96 active colours: about 14.7 FPS, but visible linear/banded shading appeared in the Xbox/orb ending.
96/128 hybrid: about 13.8 FPS, but final shading was still not ideal.
96/192/256 quality ramp: about 13.8 FPS and visually improved.

## TEST 27 — current final experiment

Purpose:
Remove palette quantisation from the final Xbox/orb/text section while retaining compression for the earlier animation.

Frame schedule at 24 FPS:
- Frames 1–23: 96-colour palette + LZ4 HC9
- Frames 24–83: 192-colour palette + LZ4 HC9
- Frames 84–107: full raw RGB565
- Raw tail duration: exactly 1.0 second

The raw tail is stored using a per-frame index-entry flag and is read directly into the existing alternating 32 KiB DMA buffers. While one DMA buffer is being transmitted, the next raw chunk is read from SD into the other buffer.

The palette/decode pipeline is unchanged for the earlier frames.

## TEST 27 asset

Filename:
ConsoleBadger_330x350_HYBRID_PALETTE_TO_RAW_RGB565_TAIL.CBP

Size:
11,040,787 bytes

Average frame payload:
103,168.3 bytes

Maximum frame payload:
231,000 bytes

The asset was validated frame-by-frame:
- palette frames: all LZ4 blocks decoded successfully
- raw frames: exact 330x350x2 byte count
- CRC checks passed for every frame

SHA-256:
978b0151134131e457ee64d655f6fe58399ba862035699b72bfa4a0ce299daee

## TEST 27 firmware

Filename:
UPCBadger_TEST_27_330x350_HYBRID_PALETTE_TO_RAW_RGB565_TAIL_DIAGNOSTIC.ino

SHA-256:
9ed5f0649f1533fd6420f02e58e15e784faf14a9424514021dd1e3dc35800e7d

Static sanity checks performed on the generated sketch:
- one LGFX class
- one setup()
- one loop()
- hybrid codec check present
- raw-frame playback branch present

No physical ESP32 performance result exists yet for TEST 27; this is the experiment awaiting the user's flash/run.

## Product finishing still outstanding

When the speed target is reached and the performance baseline is locked:

1. Remove FINAL FRAME HOLDING.
2. Smoothly fade the Xbox/PLAY YOUR WAY ending to black.
3. Hand off cleanly to the static screen.
4. Add LCD image-retention protection for long static periods:
   - inactivity timer
   - gentle fade to black
   - short black interval
   - restore static screen
   - optional tiny pixel shift between protection cycles

## Repo discipline

Do not overwrite TEST 18 or the 330x350 fast base.
Keep performance experiments separate.
Large CBP binaries should remain SD-card assets unless Git LFS/repository policy explicitly calls for committing binaries.

## Safety

Power off and unplug the ESP32 before changing wiring or soldering.
