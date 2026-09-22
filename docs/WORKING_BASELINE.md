# UPCBadger Working Baseline — 320 / 12 FPS

**Status: LOCKED WORKING PERFORMANCE BASELINE**

Verified on physical ESP32 + GC9B72 hardware on 2026-09-22.

## Display

- Panel: 360×360 GC9B72
- Active artwork/transfer window: 320×320
- Window origin: X=20, Y=20
- RGB565
- TFT SPI: 80 MHz

## Video path

- Container: CBV1
- Compression: LZ4 HC9
- Direct-DMA RGB565 byte order
- Runtime RGB565 byte swap: OFF
- SD SPI request: 40 MHz
- SD/staging/decode overlapped with TFT DMA
- Frames: 107
- Source: 24 FPS
- Working CBV: `ConsoleBadger_320_CIRCULAR_SAFE_CROPPED_DIRECTDMA_HC9.CBV`

## Verified performance

- Average frame: 83.188 ms
- Average SD: 60.945 ms
- Average decode: 11.390 ms
- Average DMA wait: 9.460 ms
- Average stage: 1.317 ms
- Slowest frame: 113.684 ms
- **Unpaced FPS: 12.021**

## Baseline rule

Do not overwrite this firmware while experimenting.

New optimisation work must be a separate file/branch and must be compared against this baseline.
