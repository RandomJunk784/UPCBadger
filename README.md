# UPCBadger

ConsoleBadger digital console badge project.

## Current development state — v1.39 / V28

**Prepared test build. v1.38 is the latest physically tested firmware.**

Current product path:
- Xbox/ConsoleBadger boot flow
- Gamer ID/profile screen
- background home Wi-Fi
- configuration portal and NVS save/persistence
- white ONLINE + Wi-Fi status row
- V28 single-layer Lumina Gamer ID background
- live profile overlay architecture for future Xbox API data

### V28 Lumina background

The Gamer ID background is now one full-screen animated asset:
- source: 2026-09-26_11-08-26_Lumina.mp4
- 97 frames retained
- 360x360
- 12 FPS
- ~8.08 second continuous loop
- runtime: `/profile_energy.CBP`
- fresh stable 256-colour RGB565 palette generated from the Lumina animation
- no dithering
- no runtime animated patch asset
- PC round-trip aggregate RGB565/display-RGB RMSE ~1.71

The V28 architecture removed the previous base+patch colour/compositor mismatch path.

### v1.39 follow-up

Based directly on physically tested v1.38.

Changes:
- remove temporary TE initials
- remove temporary circular avatar placeholder
- move ONLINE and Wi-Fi indicators left/right of the former avatar position
- keep both indicators white
- move Gamer ID text to Y=80 in the black/gloss region
- improve 12 FPS deadline handling so an overrun does not add an unnecessary full frame interval at 97 -> 0
- use 83,333 microsecond frame cadence
- poll animation service every 5 ms instead of 30 ms

The V28 `profile_energy.CBP` bytes and colours are unchanged.

### v1.39 runtime SD assets

Required:
- `ConsoleBadger_330x350_HYBRID96_192_256_HC9_TEST27B.CBP` — known-good Xbox boot
- `profile_energy.CBP` — V28 Lumina Gamer ID background

Not required by v1.39:
- `profile_bg.pbg`
- `profile_energy_patch.CBP`

Keep the legacy files in backups/archive for provenance.

### Test status

v1.39 has passed PC static/package validation but **has not yet been physically verified**.

The next physical test should verify only:
1. TE/circle are gone.
2. Gamer ID sits in the intended black/gloss region.
3. ONLINE/Wi-Fi sit cleanly left/right of centre.
4. 97 -> 0 no longer visibly pauses.
5. Lumina colours remain unchanged.

## Evidence and provenance

The Library is the project source of truth for historical versions, large binary assets, original/user-supplied media, physical LCD evidence, encoder investigation, and checksums.

The original CBP encoder source has not been recovered. `make_cbp1_reconstructed.py` is a forensic reconstruction.

See:
- docs/evidence/2026-09-26-v28-lumina-single-cbp.md
- docs/evidence/2026-09-26-v28-checksums.txt
- docs/evidence/2026-09-26-v139-ui-loop.md
- docs/evidence/2026-09-26-v139-checksums.txt
- firmware/UPCBadger_v1.37_UI_CHANGE.md
- services/profile-worker/

## Standalone demo

firmware/current/UPCBadger_PROFILE_DEMO_V1_STANDALONE.ino
