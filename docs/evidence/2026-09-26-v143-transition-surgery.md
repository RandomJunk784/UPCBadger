# UPCBadger v1.43 — Circular-energy transition surgery

Based directly on v1.42.

## Physical/visual issue
The 1–3 second circular-energy build in the 12 FPS master showed a visible
positional wobble around the top (12 o'clock) region before settling into the
later energy state.

PC inspection isolated a four-frame suspect cluster:
master frames 31, 32, 33, 34.

Frame 31 is the clearest positional excursion; frames 32–34 form the visible
recovery/swing before the animation reaches the cleaner continuation at frame 35.

## Controlled edit
Deleted master frames:
31, 32, 33, 34

Kept:
30 -> 35 -> 36 -> ...

Source-frame mapping in the original 24 FPS Seedance master:
31 -> source 62
32 -> source 64
33 -> source 66
34 -> source 68
35 -> source 70

## Asset integrity
V1.42 master:
eefce014f5a9537c64f5d8166ec57fc98f53d94ea2a9ce381a756d32e0d55c07

V1.43 master:
4af58055d33ad4a4d4f310a1e6f150ec530cd4172ea9ff1e188da533663f17ad

177/177 retained frame payloads are byte-for-byte identical to the V1.42
corresponding payloads. No retained palette entry or frame pixels were changed.

## Timing/structure
V1.42:
- 181 frames
- boot 0..103
- Gamer loop 104..180

V1.43:
- 177 frames
- boot 0..99
- Gamer loop 100..176
- Gamer loop remains 77 frames / 6.4167 s at 12 FPS

## Validation
- CBP1 structure: PASS
- frame CRCs: 177/177 PASS
- frame payload decompression: 177/177 PASS
- retained payload byte identity: 177/177 PASS
- ZIP integrity: PASS
- source brace balance: PASS

Physical LCD verification is still required.
V1.42 is retained as immediate rollback.
