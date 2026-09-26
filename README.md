# UPCBadger / ConsoleBadger — Library Catalog

Updated: 2026-09-26

## Purpose
This library is the project source-of-truth index. Technical status comes from file contents, hashes, recorded tests and physical evidence.

## Active development — 2026-09-26

Latest physically tested firmware: v1.38.
Prepared test firmware: v1.39 and v1.40.

V1.40 is the current prepared test build.

### V28 / V1.40 current state
- New single-layer Lumina Gamer ID background.
- 97 frames at 12 FPS.
- ~8.08 second continuous loop.
- Runtime background asset: profile_energy.CBP.
- Lumina palette and frame pixels are preserved; V1.40 only rephases the encoded frame order so the loop wraps at the source-frame 12 -> 13 phase.
- No profile_energy_patch.CBP required.
- No profile_bg.pbg required at runtime.
- Old five-minute black-screen retention cycle removed from V1.40.
- Future live gamerpic is intended to occupy the dark central circular well.
- V1.40 moves Gamer ID lower and smaller, removes visible Wi-Fi text, and moves ONLINE slightly outward.
- Physical LCD verification of V1.40 pending.

## Version continuity
Canonical version ZIPs are retained in 03_VERSIONS. Failed and superseded versions are not silently altered.

## Evidence
V1.40 evidence:
- V140_LOOP_CUT_F12_F13.png
- GAMERPIC_PLACEMENT_REFERENCE.png
- V140_SHA256SUMS.txt
- V1.40 package
- V1.39 physical LCD evidence

## Encoder recovery
The original CBP1 encoder source has not been recovered. make_cbp1_reconstructed.py remains a forensic reconstruction.

## Raw provenance
09_RAW_ARCHIVE/USER_DUMPS/badgerold.zip remains preserved as the original unsorted user dump.
