# UPCBadger

ConsoleBadger digital console badge project.

## Current working state

**320 / 12 FPS baseline**

- 360×360 GC9B72 panel
- 320×320 centred active artwork
- circular-safe artwork footprint
- CBV1 + LZ4 HC9
- direct-DMA RGB565 path
- SD/TFT pipeline
- verified average: **12.021 FPS**

See:
- `docs/WORKING_BASELINE.md`
- `firmware/current/UPCBadger_320_BASE_12FPS.ino`
- `docs/TEST_HISTORY.md`

## Repository structure

```
artwork/      Artwork and asset notes
docs/         Project records and experiment history
firmware/
  current/    Active working firmware
  reference/  Legacy known-good reference firmware
```

Experimental firmware that has been superseded is intentionally not kept as a pile of separate files. Its important results and failure reasons are recorded in the experiment archive.
