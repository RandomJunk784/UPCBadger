# UPCBadger V1.61 — Engineering Handoff

## Source of truth
V1.61 is derived directly from the user-supplied `UPCBadger_v1.59.ino`.
V1.59 remains immutable.

## Purpose
Make the configured GamerTag in the setup portal actually control which Xbox profile is queried, while preserving the proven animation/network memory architecture.

## Expected runtime path
BOOT → PROFILE → background Wi-Fi → CONNECTED → immediate XBL lookup using NVS GamerTag → live GamerTag/GamerScore/XUID/presence/gamerpic → restore animation → static overlay until the next scheduled refresh.

## Acceptance checks
1. Change the portal GamerTag to another known public tag.
2. Reboot.
3. Serial must show `Lookup GamerTag: <target tag>`.
4. The live GamerTag on NORTH must match the API result, not the API-key owner.
5. GamerScore must be non-zero when the target profile reports a non-zero score.
6. Presence must show ONLINE/OFFLINE only when a state was actually returned; otherwise `--`.
7. Gamerpic should cache once and not download every 30 minutes unless the URL changes or cache is missing.
8. East/West/South positions should be approximately 5 mm closer to centre than V1.59.
9. Animation must remain continuous apart from the existing deliberate network suspension/recovery window.

## V1.61 physical evidence
The latest physical serial test proved the target lookup, non-zero GamerScore, gamerpic HTTP 200/cache, and target presence HTTP 200/ONLINE path.

The remaining known failure is gamerpic rendering after the successful cache write.

## Known uncertainty
The public XBL transport path has multiple compatible lookup endpoints. V1.61 deliberately carries fallbacks and records which lookup path succeeds.

**UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.**
