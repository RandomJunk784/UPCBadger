# UPCBadger — TEST HISTORY

This file is the single record of superseded, failed and successful experiments.
Detailed experimental source files are not kept as a pile in the repository.

## CURRENT WORKING BASELINE

**firmware/current/UPCBadger_320_BASE_12FPS.ino**

Verified on the physical ESP32 + GC9B72 on 2026-09-22.

- 360×360 GC9B72 panel
- 320×320 centred transfer/artwork window at (20,20)
- circular-safe artwork footprint
- CBV1 + LZ4 HC9
- direct-DMA RGB565
- runtime RGB565 swap OFF
- SD SPI request 40 MHz
- TFT SPI 80 MHz
- SD/staging/decode overlapped with TFT DMA
- 107 frames
- source 24 FPS

**Measured:** 83.188 ms/frame average = **12.021 FPS**

Supporting measurements:
- SD: 60.945 ms/frame
- decode: 11.390 ms/frame
- DMA wait: 9.460 ms/frame
- staging: 1.317 ms/frame
- slowest frame: 113.684 ms

Working CBV:
`ConsoleBadger_320_CIRCULAR_SAFE_CROPPED_DIRECTDMA_HC9.CBV`

## KEY EXPERIMENTS

| Test | Result | Status |
|---|---|---|
| TEST 1 — SD throughput | ~1.34 MB/s | Reference |
| TEST 2 — TFT RAM → DMA | ~22.21 FPS ceiling | Reference |
| TEST 3 — first pipeline | Watchdog reset | Failed |
| TEST 3B — watchdog-safe pipeline | 4.421 → 5.009 FPS | Superseded |
| TEST 4 — known-good animation path | Correct visual path established | Superseded |
| TEST 6 — CBV1/LZ4 sequential | 5.040 FPS | Superseded |
| TEST 7C — CBV stream/coalesced | 7.125 FPS | Superseded |
| TEST 8C — 320 safe circular + pipeline | 10.933 FPS | Superseded by 12.021 FPS base |
| TEST 10 — faster LZ4 | 10.704 FPS | Rejected: total FPS fell |
| TEST 11 — 64 KiB static DMA | Linker DRAM overflow | Failed |
| TEST 11B/11C — heap 64 KiB DMA | Compile/prototype problems | Abandoned |
| TEST 12 — 320 transfer window + HC9 | **12.021 FPS** | **CURRENT BASE** |

## IMPORTANT LESSONS

- Separate SD and TFT SPI buses were important.
- Coalescing SD reads reduced filesystem overhead.
- The 320×320 transfer window reduced the amount of display data without changing the physical 360×360 panel.
- Pre-encoded/direct-DMA RGB565 removed the runtime byte swap.
- Two large static DMA buffers exceeded the ESP32's fixed DRAM budget.
- Optimising one component does not count as a win unless total frame time/FPS improves.
- Compilation success is not a working baseline; physical playback is.

## REFERENCE FIRMWARE

Legacy known-good display/video reference:
`firmware/reference/UPCBadger_KNOWN_GOOD_06_PRE_SWAP_DMA.ino`

Current production-development baseline:
`firmware/current/UPCBadger_320_BASE_12FPS.ino`

## RULE

Never overwrite the current baseline while experimenting.
Create a new test file/branch, measure it against the current baseline, and promote it only after physical playback has been verified.
