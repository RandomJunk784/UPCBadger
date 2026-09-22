# UPCBadger — WORKING BASELINE

## TEST 8C — LOCKED FALLBACK

File: UPCBadger_TEST_8C_320_SAFE_CIRCULAR_CBV_PIPELINE_STANDALONE_FIXED.ino

Status: **WORKING / PHYSICALLY TESTED**

Measured:
- 360x360 GC9B72
- 320x320 circular-feather artwork
- CBV direct-DMA RGB565
- 96 KiB staging
- 2 x 32 KiB DMA buffers
- 91.467 ms average frame
- **10.933 FPS unpaced**
- 123.012 ms slowest frame

Matching CBV:
ConsoleBadger_320_CIRCULAR_SAFE_DIRECTDMA.CBV

### Baseline policy
This is the rollback point for performance work.
Do not replace or modify the baseline in-place.
Any candidate that does not physically beat 10.933 FPS while retaining correct artwork remains experimental.
Current goal: **12 FPS without clipping or square-edge artefacts.**