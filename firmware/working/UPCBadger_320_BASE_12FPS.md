# UPCBadger — WORKING BASE: 320 / 12 FPS

Status: WORKING PERFORMANCE BASELINE

Verified on physical ESP32 + GC9B72 hardware on 2026-09-22.

## Locked playback configuration

- Display panel: 360x360 GC9B72
- Active artwork footprint: 320x320, centred at X=20, Y=20
- Circular-safe artwork / no square border
- Pixel format: RGB565
- CBV container: CBV1
- Compression: LZ4 HC9
- Direct-DMA RGB565 byte order
- TFT SPI: 80 MHz
- SD SPI request: 40 MHz
- Pipeline: SD/staging/decode overlapped with TFT DMA
- Runtime RGB565 byte-swap: OFF
- Frames: 107
- Source rate: 24 FPS
- Working CBV asset: ConsoleBadger_320_CIRCULAR_SAFE_CROPPED_DIRECTDMA_HC9.CBV

## Verified performance

Average frame: 83.188 ms
Unpaced FPS: 12.021
Average SD: 60.945 ms
Average decode: 11.390 ms
Average DMA wait: 9.460 ms
Average stage: 1.317 ms
Slowest frame: 113.684 ms

## Baseline rule

Do not overwrite this baseline while experimenting.

Future optimisation tests must be separate copies and must be compared against this result.

The physical 360x360 display and 320x320 visual footprint are now the working visual target unless a later test proves a better result without clipping or visible boxing.
