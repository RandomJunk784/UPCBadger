# UPCBadger Experiment Archive

This is the single index for superseded and failed experiments.

## Superseded performance paths

### TEST 7C — CBV1 stream-coalesced
320-safe CBV with 96 KiB staging and the earlier pipeline.
Verified at 7.125 FPS average.
Superseded by the later 320-window path.

### TEST 8C — 320 safe circular + DMA pipeline
Reached 9.934 FPS.
Superseded by the 320 transfer-window optimisation.

### TEST 10 — optimised LZ4 decoder
Reduced decode time, but increased DMA wait enough to lower total performance to 10.704 FPS.
Retained only as historical experiment context.

### TEST 11 / 11B
64 KiB DMA-buffer experiments were abandoned because the ESP32 could not provide two 64 KiB DMA buffers in the available DMA-capable memory.

## Current outcome

The active working baseline is documented in `docs/WORKING_BASELINE.md` and the verified firmware lives in `firmware/current/`.

Do not resurrect archived experiments directly. Start a new branch/file and compare against the working baseline.
