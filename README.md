# UPCBadger / ConsoleBadger

Updated: 2026-09-29

## Active development — v2.33

Latest controlled gamerpic test, based directly on the protected v2.31 physical recovery point.

### v2.33 changes

- Keeps the proven Xbox 208x208 gamerpic source.
- Removes the 72x72 intermediate decode stage.
- Tests direct 208x208 -> 115x115 RGB565 decode.
- Keeps the proven 115px circular presentation.
- Retains the exact v2.07 R/B correction.
- Gamerpic outer green ring remains removed.
- No intentional changes to the GamerID, GamerScore, status bars, Wi-Fi, XBL, SD, CBP, TLS/memory recovery, animation timing, display geometry or hardware pins.
- v2.32 direct 208x208 -> 127x127 decode failed at the PNG decode stage and produced no gamerpic.

### Protected v2.31 recovery point

V2.31 remains the protected physical recovery point and is not overwritten by the v2.33 experiment.

### Development rule

Subsequent revisions should change one controlled element at a time and be physically compiled/tested before becoming the new baseline.

## Secret handling

Real API keys must stay out of GitHub.

Use local-only secrets and safe templates. Never commit a live OpenXBL API key.

## Safety

Before wiring, soldering, or physical modification:

**UNPLUG THE ESP32 FIRST.**
