# UPCBadger — WORKING BASELINE: TEST 18 Palette256 / 13.536 FPS

Status: LOCKED WORKING PERFORMANCE BASELINE

Verified on physical ESP32 + GC9B72 hardware on 2026-09-22.

## Locked playback configuration

- Display panel: 360x360 GC9B72
- Active video window: 324x360 at X=18, Y=0
- Visual composition: 10% zoom-out / orb-fixed candidate
- Pixel format on TFT: RGB565
- Container: CBP1
- Codec: 256-colour per-frame RGB565 palette + independent LZ4 HC9
- Palette: 256 RGB565 entries (512 bytes/frame)
- Encoded index block: 16 KiB
- TFT SPI: 80 MHz
- SD SPI request: 40 MHz
- Pipeline: SD/staging/decode overlapped with TFT DMA
- DMA buffers: 2 x 32 KiB
- Staging buffer: 96 KiB
- Runtime RGB565 byte-swap: OFF
- Frames: 107
- Source rate: 24 FPS

## Verified performance

- Average frame: 73.878 ms
- Unpaced FPS: 13.536
- Average SD: 47.254 ms
- Average decode: 20.399 ms
- Average DMA wait: 5.445 ms
- Average stage: 0.680 ms
- Slowest frame: 92.742 ms

## Comparison

The previous 324x360 RGB565/LZ4 CBV path measured about 10.23 FPS.

TEST 18 therefore improved measured playback to 13.536 FPS while retaining the 360x360 physical display and 324x360 active video window.

## Locked asset

ConsoleBadger_360_PALETTE256_HC9.CBP

The CBP asset is currently maintained alongside the working local SD test files. Its validated local build is the asset used for this baseline.

## Baseline rule

Do not overwrite this baseline while experimenting.

Future optimisation tests must be separate copies and must be compared against TEST 18.

The 320 / 12.021 FPS CBV remains retained as the fallback compatibility baseline.

## Next optimisation work

Future performance work should target TEST 18 rather than restarting the playback architecture.

Potential targets:

- SD transport/read path
- palette/index read efficiency
- LZ4 decode hot path
- DMA scheduling

Do not change the TFT pinout or visual geometry during performance-only tests unless explicitly testing a new artwork footprint.

## Hardware / safety

TFT:
- MOSI 23
- SCLK 18
- CS 21
- DC 22
- RST 4

SD:
- MOSI 27
- SCLK 25
- MISO 26
- CS 13

Power off / unplug the ESP32 before changing wiring or soldering.