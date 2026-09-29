# UPCBadger V2.30 — Build Evidence
Date: 2026-09-29

Base: v2.28, latest physically working baseline.

Implemented:
- Real target-XUID Xbox Live presence poll every 5 minutes.
- GamerTag/GamerScore/gamerpic profile sync remains 20 minutes.
- Wi-Fi state check remains 60 seconds.
- Animation remains 12 FPS.
- Existing cached status-bar compositor consumes the new presence state.

Validation:
- v2.28 source transformed only with revision metadata, one presence timer/state, the new presence service, a timer reset after successful full profile sync, and one loop call.
- Source delimiter sanity check passed.
- v2.30 is not yet physically tested; v2.28 remains rollback baseline.
