# UPCBadger v1.40 — V28 UI / Retention Revision
Date: 2026-09-26

## Base and physical evidence
V1.40 is based directly on physically tested v1.39 and the V28 Lumina single-layer background.
User requested removal of the old retention flash after reporting that repeated flashing was uncomfortable.

## V1.40 changes
1. Gamer ID moved from Y=80 to Y=100.
2. Font2 used normally; Font0 fallback when the rendered username exceeds 300 pixels.
3. TE initials remain removed.
4. Cyan/blue avatar placeholder remains removed.
5. ONLINE moved outward to the left; dot/text retained in white.
6. Visible Wi-Fi text removed; white Wi-Fi beacon retained at the right for visual placement/state.
7. Old burn-protection routine removed completely: no five-minute timer, black hold, fade cycle or 2-pixel retention shift.
8. Lumina loop remains 97 frames @ 12 FPS.
9. Only encoded frame-record order changed: new order begins at old frame 12 and ends at old frame 11, so the visible wrap is old frame 11 -> old frame 12 / source frames 12 -> 13.

## CBP integrity
The V1.40 profile asset was created by reordering the existing V28 encoded frame payloads only.
No frame payload bytes or palette entries were altered.
The multiset of encoded frame-payload hashes is identical to V28.

V1.40 profile_energy.CBP SHA-256:
0b5fa8cac53f35d08f2319de55245895462fcfd11d2f792cfb915eac1442f534

Original V28 profile_energy.CBP SHA-256:
1f12cd6cad81253bd32cf9ac76065b7895af842c86d249c69680787de93cd025

The asset hash changes only because record ordering changed.

## Runtime SD assets
- ConsoleBadger_330x350_HYBRID96_192_256_HC9_TEST27B.CBP — known-good boot
- profile_energy.CBP — V28 Lumina Gamer ID background
Legacy assets not required at runtime: profile_bg.pbg and profile_energy_patch.CBP.

## Package
UPCBadger_v1.40.zip
Outer SHA-256:
8a949e775aa970d0bc5d0f642516054bd8a48fa3a215d8b9782c1a4964124246

## Validation
- source braces balanced: PASS
- old retention function/timer symbols absent: PASS
- Wi-Fi text absent from runtime profile status helper: PASS
- ZIP integrity: PASS
- internal SHA-256 manifest generated
- CBP payload set preserved exactly; only cyclic record ordering changed
- physical LCD verification pending

## Gamerpic placement
The dark central circular well in the supplied physical LCD reference is the future live Xbox gamerpic placement region. No artificial avatar circle is drawn in V1.40.