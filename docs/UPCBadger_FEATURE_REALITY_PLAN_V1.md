# UPCBadger — Feature Reality Plan V1

## Core product
UPCBadger remains fundamentally:
1. A polished boot screen/animation.
2. A compact profile/detail pane.
3. Optional local software features layered around those two things.

The product must remain useful if every optional feature is disabled.

## Engineering rule
**£0 wherever possible, but minimise total cost (cash + wasted time).**

Preferred order:
1. Use hardware/software already owned.
2. Borrow.
3. Beg/borrow broken or unwanted hardware for experiments.
4. Simulate/recreate the required signal/data.
5. Buy cheap used hardware when it is demonstrably cheaper than several days of engineering.
6. Buy new hardware only when justified.

Do not buy parts merely to explore an idea.

## Privacy rule
Features should work locally wherever practical.
No unnecessary collection of personal/sensitive data.
No hidden telemetry.
No requirement for a cloud account unless a user explicitly chooses a service that needs one.
QR codes may point users to public resources/searches; UPCBadger does not need to know whether they were scanned.

## Persistent settings
No battery is required for configuration.

Use ESP32 non-volatile flash/NVS for:
- Gamertag/profile identifiers
- Platform selection
- Automatic detection enabled/disabled
- Theme selection
- Display settings
- Museum progress
- Setup state

Write settings only when they change; do not repeatedly write dynamic values.

The recessed service button remains the recovery/program/factory-reset control. Planned long hold: approximately 15 seconds.

## Multi-platform profile concept
A user may configure multiple local profiles, for example:
- Xbox / Gamertag
- PlayStation / PSN identity
- Nintendo / local profile identity
- Custom/retro platform

Automatic platform detection chooses the relevant profile/theme when confidence is sufficient.

If detection fails or is disabled:
- show a simple manual platform selection menu
- persist the chosen default locally

Fallback must never prevent the normal Badger boot.

## Controller detection experiment
Goal: determine whether modern controllers expose sufficiently distinctive Bluetooth information for UPCBadger to choose a platform boot animation.

Target platforms:
- Xbox
- PlayStation
- Nintendo
- potentially retro/other systems through manual selection

Existing standalone experiment:
firmware/experiments/UPCBadger_CONTROLLER_DETECT_V1_STANDALONE.ino

It scans BLE advertisements and Bluetooth Classic discovery on the original ESP32 DevKit 1 and logs:
- device name
- address
- RSSI
- manufacturer data
- service UUID/data where available
- Bluetooth Classic class-of-device
- fingerprint material

It does not intentionally connect to controllers or store their addresses.

Test methodology:
- baseline with controllers off
- wake one controller
- repeat several times
- compare stable fields
- repeat with different controller families

Success does NOT require identifying a specific controller model. We only need enough stable evidence to choose:
XBOX / PLAYSTATION / NINTENDO / UNKNOWN

Unknown falls back to default/manual selection.

Important uncertainty:
A controller may use Bluetooth Classic rather than BLE, may not advertise continuously, may expose limited data, may randomise addresses, or may only reveal useful identity after a connection. These are experiments to resolve, not assumptions.

## Future automatic boot flow
Proposed runtime:

POWER
→ initialise ESP32
→ load local settings
→ listen briefly for controller/platform evidence
→ confidence sufficient?
   YES → select matching profile/theme
   NO → use saved/default platform
→ play selected boot animation
→ normal profile/detail pane

Detection should be time-bounded. The Badger must never sit waiting for Bluetooth and delay the console-style boot unnecessarily.

Potential future refinement:
Use controller wake activity, recent sightings and RSSI to infer the active controller when several registered controllers are nearby.

## Manual fallback
Example:

SELECT PLATFORM
1. XBOX
2. PLAYSTATION
3. NINTENDO
4. ATARI/RETRO
5. CUSTOM

Exact menu can change later.

## Detail pane feature pool
Prioritise free/local features:
- profile identity
- platform/theme identity
- clock/date
- local display settings
- Badger status
- Easter egg/museum progress
- offline gaming facts/trivia
- QR discovery cards

Potential external service features, such as weather, must be optional and evaluated for:
- zero/low cost
- rate limits
- privacy
- reliability
- whether the feature is genuinely worth the added complexity

## Easter Egg Madness / Gaming History Museum
Separate future subsystem; do not destabilise profile/boot firmware.

Concept:
- rare discovery event
- short era-appropriate attention/error sound
- EGG X OF 10
- user presses main button to enter
- chronological exhibits
- one-time choice of direction (forward/backward)
- no skipping between exhibits
- progress stored locally
- historical content recreated/original where possible
- QR/search discovery card at the end
- older eras can run longer; modern eras target shorter runtimes because of graphical complexity

Existing architecture sketch:
docs/EASTER_EGG_MADNESS.md

## Buttons
Proud main button:
- normal short press: display dim function
- museum prompt: enter exhibit
- exhibit: advance pages where instructed
- rapid interaction may be used for QR/discovery behaviour as defined later

Recessed service/program button:
- programming/setup
- factory recovery/reset
- approximately 15-second recovery hold

Exact button state machine to be implemented only after profile setup is stable.

## Audio philosophy
Normal Badger boot can remain silent except for a very short optional boot/attention cue because the console itself is booting and making noise.

Museum events may use:
- short era-appropriate discovery/error sound
- synchronised exhibit audio
- original/recreated audio rather than ripped copyrighted game audio

## Product architecture
Keep these as separate modules/state machines:
- Profile
- Platform Detection
- Theme/Boot Selection
- Boot Player
- Detail Pane
- Button/Input Manager
- Persistent Settings
- Museum/Easter Eggs

A failure in an optional subsystem must fall back to the normal Badger boot.

## Immediate priorities
1. Finish/test profile identity/setup.
2. Test controller Bluetooth fingerprint prototype with borrowed controllers.
3. Determine whether passive detection is sufficiently reliable.
4. If viable, build a tiny local platform-fingerprint table.
5. Add automatic theme selection without altering the known-good animation renderer.
6. Add manual fallback and persistent platform selection.
7. Polish the boot/profile experience.
8. Add optional free features.
9. Integrate Easter Egg Madness last.

## Golden rule
**Do not build the whole dream at once.**
Every experiment must answer one useful question and preserve the known-good baseline.
When something cannot be done with the available hardware/software, stop and identify the cheapest route around it rather than forcing a purchase.


## Easter Egg delivery model — provisional
The museum does not have to be exhausted in one session.

- Easter eggs may be delivered at most approximately one per calendar month.
- The exact month/egg selection can be pseudo-randomised locally so discovery remains unpredictable.
- The device stores the delivered/seen state in NVS.
- Once an egg has been delivered, it remains available to the user; the system does not need to repeat it unless deliberately designed to do so.
- The core historical sequence remains ordered for the historical museum exhibits. The monthly delivery mechanism controls **when** an exhibit becomes available, not its historical position.
- Special seasonal eggs are separate from the historical sequence.
- **Halloween and Christmas eggs are not tied to a particular gaming era.** They may draw from any decade/platform represented in the museum and can deliberately jump across eras.
- Seasonal eggs can therefore be short, unusual, humorous or cross-era without disrupting the chronological museum story.
- The provisional total should be treated as more than the original 10-egg museum: a base historical collection plus seasonal/special eggs.
- Delivery must be deterministic enough to prevent accidental duplicate delivery, but unpredictable enough that the user cannot simply know the next exhibit from the calendar alone.
- No server is required for scheduling; the ESP32 can use its local stored state. If reliable real-world date/time is unavailable, seasonal handling can fall back to configured date or be disabled rather than making a false assumption.
