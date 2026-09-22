# UPCBadger — TEST HISTORY / WINS & FAILURES

## Current working baseline

**WORKING BASELINE: TEST 8C — 320 SAFE-CIRCULAR CBV + TRUE DMA PIPELINE**

Known-good measured result:
- 360x360 GC9B72 canvas
- 320x320 circular-feather artwork footprint
- CBV compressed video
- direct-DMA RGB565 path
- 96 KiB compressed staging
- 2 x 32 KiB decoded DMA buffers
- average frame: **91.467 ms**
- unpaced rate: **10.933 FPS**
- slowest frame: **123.012 ms**
- average SD: 61.445 ms
- average decode: 23.932 ms
- average DMA wait: 4.626 ms
- average staging: 1.384 ms
- CBV size: **8,865,685 bytes**

This is the fallback baseline. Future optimisation work must preserve a copy of this code and must not replace it until a new build has been physically tested.

---

## Test progression

### TEST 1 — Raw SD throughput
- 16 KiB: 1.339 MB/s
- 32 KiB: 1.340 MB/s
- 64 KiB: 1.340 MB/s
- 128 KiB allocation failed

Conclusion: larger read buffers did not improve throughput; SD alone did not explain the whole playback problem.

### TEST 2 — GC9B72 RAM → DMA
- complete 360x360 frame transfer: about 45.018 ms
- measured ceiling: about 22.21 FPS
- transfer rate: about 5.758 MB/s
- initial full-frame DMA allocation failed; chunked transfer subsequently worked

Conclusion: TFT transport had substantially more capacity than the original SD/decode/display path.

### TEST 3 — First SD/TFT pipeline
Failure: producer task starved CPU 0 and the task watchdog rebooted the ESP32.

### TEST 3B — Watchdog-safe pipeline
- sequential: 24,204 ms / 4.421 FPS
- pipelined: 21,363 ms / 5.009 FPS
- improvement: **11.7%**

Conclusion: overlapping SD and TFT work genuinely helped.

### TEST 4 — Known-good animation path
- correct ConsoleBadger boot animation established
- separate SD/TFT SPI
- RGB565 direct-DMA path
- average processing: 207.94 ms/frame
- processing-only rate: 4.81 FPS

Conclusion: this established the correct visual path before compression optimisation.

### TEST 6 — CBV1 / LZ4 / sequential
- CBV size: 13,359,041 bytes
- 107 frames
- average frame: 198.432 ms
- average SD: 135.164 ms
- average decode: 24.252 ms
- average DMA: 25.954 ms
- unpaced: **5.040 FPS**

Failures included a missing-CBV-file run and a missing FRAME_PIXELS compile definition; both were corrected.

### TEST 7 — Frame-coalesced CBV
Initial failure: 192 KiB compressed staging allocation failed. Largest free 8-bit block was only 110,580 bytes.

### TEST 7B — 96 KiB streaming staging
Initial compilation failed because Arduino generated prototypes before StageReader was declared.

After correction:
- average frame: 157.002 ms
- unpaced: **6.369 FPS**

Later run:
- CBV size: 11,068,335 bytes
- average frame: 140.343 ms
- average SD: 77.680 ms
- average decode: 21.768 ms
- average swap: 12.746 ms
- average DMA: 25.954 ms
- unpaced: **7.125 FPS**

Conclusion: streaming/coalescing was a substantial improvement.

### TEST 8 — 320 SAFE-CIRCULAR CBV + DMA pipeline
- average frame: 100.664 ms
- unpaced: **9.934 FPS**

Major visual win: 320x320 artwork with a circular feather to black removed the obvious square-edge artwork box while retaining the 360x360 display canvas.

### TEST 8C — 320 SAFE-CIRCULAR / TRUE DMA PIPELINE
**WORKING BASELINE**

- CBV size: **8,865,685 bytes**
- average frame: **91.467 ms**
- average SD: 61.445 ms
- average decode: 23.932 ms
- average DMA wait: 4.626 ms
- average staging: 1.384 ms
- slowest frame: 123.012 ms
- unpaced: **10.933 FPS**

Conclusion: first confirmed build above 10 FPS and current rollback point.

### TEST 9 — 320 SAFE DIRECT-DMA HC9 pipeline
Continued the 320/direct-DMA/HC9 optimisation line. Superseded by TEST 10; TEST 8C remains the named fallback baseline.

### TEST 10 — Optimised LZ4
- average frame: 93.419 ms
- average SD: 61.488 ms
- average decode: **19.288 ms**
- average DMA wait: **11.172 ms**
- average staging: 1.385 ms
- slowest frame: 124.341 ms
- unpaced: **10.704 FPS**

Change: optimised LZ4 overlap copy enabled; runtime RGB565 swap remained OFF.

Conclusion: decoder got faster but DMA waiting increased enough that total FPS was worse than TEST 8C. Keep experimental, do not promote.

### TEST 11 — 64 KiB DMA buffers
Failure: linker reported .dram0.bss overflow; DRAM segment exceeded the region by 32,272 bytes.

Lesson: larger DMA buffers must be allocated from runtime DMA-capable heap rather than static .bss.

### TEST 11B — heap-allocated 64 KiB DMA experiment
Compilation failure: Arduino auto-prototype generation again saw StageReader after functions using it. The resulting errors cascaded into redeclared-function messages.

This is a source-order/prototype problem, not evidence against the heap-DMA architecture.

### TEST 11C
Purpose: same 64 KiB DMA experiment with the StageReader declaration/prototype issue corrected.
Status: **not yet physically tested**.

---

## Established architecture
- TFT MOSI 23 / SCLK 18 / CS 21 / DC 22 / RST 4
- SD MOSI 27 / SCLK 25 / MISO 26 / CS 13
- TFT SPI 80 MHz
- SD request 40 MHz
- LovyanGFX
- separate SPI buses
- RGB565 direct-DMA byte order
- no runtime RGB565 swap
- SD → compressed staging → LZ4 decode → ping-pong DMA buffers → TFT
- 360x360 display canvas with 320x320 circular-feather artwork

## Rules
1. Never overwrite TEST 8C.
2. Keep a physical copy of TEST 8C and its matching CBV.
3. Every optimisation is measured against TEST 8C.
4. Compilation success does not qualify as a working baseline; physical playback does.
5. Do not sacrifice the circular feather/masking solution for FPS.
6. Record SD, decode, DMA wait, staging and total frame time.
7. If an optimisation improves one component but worsens total FPS, keep it experimental.
8. Arduino sketch folders must contain only the intended .ino unless deliberate multi-file compilation is being used.
9. Before changing electronics/wiring: UNPLUG THE ESP32.

## Current target
Minimum practical target: **10–12 FPS fullscreen with no logo clipping and no visible square black box.**
Current confirmed: **10.933 FPS with TEST 8C.**
Remaining gap to 12 FPS: about 1.07 FPS.