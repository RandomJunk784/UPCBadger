# UPCBadger — Easter Egg Madness

## Concept
UPCBadger contains a hidden chronological Gaming History Museum. It is deliberately rare, surprising and privacy-preserving. The museum is not a normal daily feature and does not collect user data.

The first discovery establishes the journey direction once:
- FORWARD — oldest to newest
- BACKWARD — newest to oldest

After that choice, exhibits are presented strictly in sequence. The user cannot skip between exhibits. The firmware persists progress locally.

Target museum: 10 exhibits, roughly five-year periods from 1970–1974 through 2015–2019.

## Discovery flow
1. Badge is in its normal quiet state.
2. A rare museum event becomes eligible according to the current local progress counter.
3. Badger plays a short, era-appropriate fake error/fault sound. This is the attention grabber; it must initially sound like an unexpected system/console fault rather than an obvious Easter egg announcement.
4. Screen shows an EGG X OF 10 / EGG FOUND prompt and asks the user to press the main button to investigate.
5. User presses the proud main button.
6. The current historical exhibit begins.
7. The exhibit contains original/recreated visual material, era-specific audio, short historical context and a QR/search discovery card.
8. History pages use the main button to turn pages. The button is not a skip control between exhibits.
9. After the exhibit is completed, local progress advances to the next exhibit in the chosen direction.
10. If the device loses power during an exhibit, the current exhibit remains the next exhibit; progress only advances after completion.

## Button behaviour
### Normal badge
- Short press: display dim function.
### Museum discovery prompt
- Short press: enter/view the current exhibit.
### During exhibit
- Button advances history pages only where explicitly shown.
- No skipping to later/earlier museum exhibits.
### QR/discovery card
- QR remains visible for approximately 10 seconds.
- It encodes a public search/rabbit-hole destination; no UPCBadger account or tracking is required.
### Recessed service button
Separate from the museum. Used for programming, factory setup/reset and recovery. Planned long-hold recovery remains approximately 15 seconds.

## Audio philosophy
Normal boot is silent. The console and Badger receive power together, so UPCBadger does not need to compete with console startup audio.
The museum uses audio as a deliberate event signal:
- short fake-error sound announces the rare discovery
- sound palette changes with the five-year period
- exhibit audio can be synchronised to the animation
- sounds should be original/recreated, not ripped game audio
The existing user-supplied audio reference should be analysed later for the final playback format and audio pipeline.

## Exhibit timeline
| Exhibit | Period | Historical direction |
|---:|---|---|
| 1/10 | 1970–1974 | Early arcade/computer gaming |
| 2/10 | 1975–1979 | Arcade growth and early home gaming |
| 3/10 | 1980–1984 | Arcade boom / iconic early games |
| 4/10 | 1985–1989 | 8/16-bit home gaming |
| 5/10 | 1990–1994 | 16-bit era, arcade/home transition, PC gaming |
| 6/10 | 1995–1999 | 3D transition and console wars |
| 7/10 | 2000–2004 | PS2/Xbox/GameCube era and online growth |
| 8/10 | 2005–2009 | HD consoles, Wii and online gaming |
| 9/10 | 2010–2014 | Minecraft and modern/social gaming |
| 10/10 | 2015–2019 | Streaming/social era and contemporary gaming |

The exact games and sequence within each exhibit remain to be storyboarded.

## Runtime philosophy
Older periods can run longer because their visual language is simpler and easier to recreate convincingly. Modern periods should target roughly 10 seconds because their graphics approach the current boot-animation complexity and therefore consume more playback/storage budget.
Indicative targets: 1970s 15–20+ seconds; 1980s 15–20 seconds; 1990s 12–15 seconds; 2000s 10–12 seconds; 2010s about 10 seconds.

## Privacy / networking
The museum itself must be fully offline-capable. QR codes should point to public search/rabbit-hole destinations. UPCBadger does not need to know whether a QR code was scanned. No museum analytics, tracking or sensitive data collection.

## Integration plan
Do not integrate into the known-good profile/boot playback path yet. Profile setup and the current product branch remain the priority.
Future integration should use a separate museum state machine and asset namespace so Easter Egg Madness cannot destabilise the known-good display path.
Suggested future modules: MuseumState, MuseumScheduler, MuseumDiscoveryUI, MuseumExhibitPlayer, MuseumQR, MuseumStorage.
The museum should be callable from the normal application as a self-contained event, then return control cleanly to the existing profile/dashboard state.

## Design principle
The Badger should make the user curious, not make them manage a feature.
The user discovers an unexpected sound, investigates, experiences one historical slice, and is given a route into the wider history. The next exhibit remains locked until its place in the sequence is reached.