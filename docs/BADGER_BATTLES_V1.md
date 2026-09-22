# UPCBadger — Badger Battles V1

## Concept
A separate personality/event subsystem in which UPCBadger unexpectedly enters a short arcade-style fight against a recognisable gaming character.

This is not a conversation or character cameo. It is a fight.

Examples:
- Badger vs Ryu
- Badger vs Pac-Man
- Badger vs Sonic
- Badger vs Master Chief
- Badger vs Space Invader
- Badger vs Tetris
- Badger vs Minecraft character

## Design goal
Make the battles feel like tiny, polished arcade encounters rather than low-FPS video playback.

Target:
- roughly 5–15 seconds normally
- large readable animation poses
- limited keyframes
- screen shake/glitch effects where appropriate
- short themed audio
- simple but convincing action
- clear winner/loser ending

The hardware's limited FPS is a design constraint, not something to hide. Strong keyframes, timing, sound and effects should carry the spectacle.

## No draws
Every battle has a definite winner and loser.

The result must be believable within the fictional/game rules of the matchup. Do not choose outcomes purely at random.

## Matchup model
Each opponent gets a small local matchup definition containing:
- character/identity
- era
- entry animation
- Badger attack/defence options
- opponent attack/defence options
- plausible win conditions
- plausible loss conditions
- winner-specific ending
- loser-specific ending
- audio/event cues
- optional difficulty/weighting

Example conceptual matchups:
- Ryu: default outcome Ryu wins through a decisive fighting-game attack.
- Sonic: default outcome Sonic wins through overwhelming speed, unless the scene provides a credible trap.
- Pac-Man: either side can win through a clearly shown power-up/chase outcome.
- Tetris: falling blocks can trap Badger; Badger can win by surviving/clearing the sequence.
- Master Chief: default outcome Master Chief wins unless the scene gives Badger a credible environmental advantage.
- Space Invader: arcade-style retaliation can credibly favour Badger.

## Event flow
NORMAL IDLE
→ battle trigger
→ INCOMING CHALLENGER
→ opponent reveal
→ short fight
→ decisive action
→ winner reveal
→ loser reaction
→ optional search/QR card
→ return to normal Badger

The battle must never block normal operation permanently.

## Triggering
Battles should be rare personality events, separate from the chronological Gaming History Museum.

Possible triggers:
- random idle personality event
- seasonal event
- hidden button combination
- special museum completion
- platform-specific event

Exact probability is TBD.

## Relationship to Gaming History Museum
The Museum teaches gaming history in chronological order.

Badger Battles are independent entertainment events and may jump between eras.

A 1980s opponent can fight a modern Badger without changing museum chronology.

## Search/discovery
After selected battles, optionally show a short search term or QR discovery card.

Example:
SEARCH: RYU STREET FIGHTER

This is informational/discovery content, not tracking. UPCBadger does not need to know whether the QR code was scanned.

## Software architecture
Create a standalone state machine:

BATTLE_IDLE
BATTLE_INTRO
BATTLE_FIGHT
BATTLE_DECISIVE
BATTLE_RESULT
BATTLE_DISCOVERY
BATTLE_EXIT

Keep rendering, input, audio and battle data separate.

Recommended future BattleDefinition fields:
- opponent ID
- era
- intro frames
- fight frames
- win path
- loss path
- audio cues
- result text
- discovery text
- runtime limits

The core Badger firmware should be able to ignore the entire subsystem if it is unavailable or disabled.

## Development order
1. Build one generic battle renderer using placeholder graphics.
2. Prove the state machine on the current ESP32 + GC9B72.
3. Create one complete original test opponent.
4. Prove screen effects and audio timing.
5. Add individual matchup definitions.
6. Add rare random triggering.
7. Add discovery/QR ending.
8. Integrate into the main personality engine only after the standalone battle works reliably.

## Asset/IP note
For a product intended for sale, use original assets or appropriately licensed material. Character names can be used as development placeholders; final commercial presentation should be reviewed for rights.

## Golden rule
A battle should feel like a tiny complete event:
recognition → threat → action → decisive outcome → reaction → return.

No draws. No endless fights. No damage to the console or user data. No network requirement.
