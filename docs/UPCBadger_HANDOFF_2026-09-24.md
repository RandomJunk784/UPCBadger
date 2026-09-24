# UPCBadger / ConsoleBadger — Engineering Handoff
## 2026-09-24

## CURRENT OBJECTIVE

Get the integrated product working as one firmware:

POWER ON
→ ConsoleBadger/Xbox boot animation
→ final boot frame / handoff
→ Gamer ID / profile screen
→ background home Wi-Fi
→ browser-based first-time configuration when required
→ normal product operation

Immediate blocker: the integrated player previously failed at SD initialisation.

---

# HARDWARE — LOCKED

## ESP32
DOIT ESP32 DevKit 1

## TFT / GC9B72
- MOSI 23
- SCLK 18
- CS 21
- DC 22
- RST 4
- TFT SPI: 80 MHz
- LovyanGFX
- DMA enabled
- setSwapBytes(false)

## SD
- MOSI 27
- SCLK 25
- MISO 26
- CS 13
- SD SPI: 40 MHz
- Separate HSPI bus

Before wiring changes or soldering:
UNPLUG THE ESP32 FIRST.

---

# TOOLCHAIN

Known:
- Arduino IDE 2.3.10
- LovyanGFX 1.2.29
- ESP32 board package previously recorded as 3.3.11

---

# PROTECTED BASELINES

## Display/video baseline
`UPCBadger_HQ_OPTIMISED_TEST_06_PRE_SWAP_DMA.ino`

Known-good picture/performance reference. Do not overwrite.

## Product source evidence
`UPCBadger_PRODUCT_MODE_V11_KNOWN_GOOD_BOOT_PLUS_PROFILE.ino`

Current Library reference for product integration.

---

# VIDEO / PLAYER

The Library V11 product source contains:
- 360x360 physical panel
- 330x350 active video window
- CBP1 palette/LZ4 playback
- 2 x 32 KiB decoded DMA buffers
- 256-entry RGB565 palette
- 107-frame CBP index table
- 96 KiB compressed staging buffer
- LovyanGFX DMA
- SD on separate HSPI

CBP asset:
`/ConsoleBadger_330x350_HYBRID96_192_256_HC9_TEST27B.CBP`

Do not reconstruct the decoder/player from memory.

---

# PRODUCT FLOW

Library V11 product flow:

POWER ON
→ TFT
→ SD + CBP index
→ Xbox boot
→ NVS/product settings
→ if unconfigured: SoftAP configuration
→ if configured: Gamer ID/profile screen
→ background home Wi-Fi
→ periodic profile service

Config button:
- GPIO32
- 3 s hold = configuration mode
- 15 s hold = factory reset

Setup service objects are intended to exist only when setup mode is entered.

---

# EVIDENCE — SD / WIFI COMPATIBILITY

`UPCBadger_SD_WIFI_COMPAT_TEST_V1.ino`

Proven on project hardware:
- TFT init
- SD init at 40 MHz HSPI
- NVS
- Wi-Fi initialisation
- reached TEST COMPLETE

Important:
- TFT CS HIGH before SD
- SD CS HIGH before SD
- order: TFT → SD → NVS → Wi-Fi
- no CBP playback in this compatibility test

This proves the basic TFT/SD/NVS/Wi-Fi stack can coexist.

---

# EVIDENCE — v1.09

Observed:
- before staging: 314948 free heap
- after 96 KiB staging: 212544
- after TFT: 209676
- SD.begin(): PASS
- after SD: 179332
- largest block after SD: 65524

Methodology limitation:
v1.09 did not execute the live playback path, so it did not prove the full retained runtime player footprint.

Correct interpretation:
v1.09 proves SD initialisation succeeds in that diagnostic environment.

---

# EVIDENCE — v1.13

Observed:
`[SD] Heap before init: 104240`
`[SD] ERROR: SD init failed.`

This reproduced the failure with the live product/player source.

Inference:
the integrated live player entered SD initialisation with much less available heap than the v1.09 diagnostic.

---

# EVIDENCE — v1.14

Observed:
`[SD] Heap before init: 206644`
`[SD] SD.begin() OK`
`[SD] Xbox CBP index loaded.`
`[MEM] ERROR: staging allocation failed after SD init.`

This is the most important result so far.

PROVEN:
- full player globals can coexist with SD initialisation
- SD hardware/path is not the current blocker
- the fault has moved to the 96 KiB staging allocation

v1.14 therefore successfully moved the failure point past SD.

---

# CURRENT ENGINEERING POSITION

The current strongest hypothesis is no longer:
“SD cannot initialise with the player.”

That is disproved by v1.14.

The current problem is:
the required 96 KiB contiguous staging allocation cannot be obtained after SD/CBP initialisation.

The player source's streaming code uses the staging buffer for palette data and compressed CBP blocks. The largest block described by the source is 16 KiB of raw index data plus an 8-byte block header. The current 96 KiB staging size is therefore a generous streaming reservoir, not a direct requirement for one decoded block.

This is still an engineering inference until the player is successfully run with a smaller staging buffer.

---

# PREVIOUS EXPERIMENTS — STATUS

## v1.05
WiFi.h integration test; repeated reset.
Historical regression evidence.

## v1.06
Checkpoint diagnostics; reached staging/TFT then SD failure.
Useful for locating failure stage.

## v1.07
Deeper diagnostics; methodology not accepted as a clean controlled experiment.
Historical reference only.

## v1.08
WiFi.h + 40→25 MHz SD experiment; repeated reset.
Does not prove SD speed was the cause.

## Product A/B compatibility test
`UPCBadger_SD_PRODUCT_LAYER_A_B_TEST_V2.ino`
Proved TFT + 96 KiB staging + SD + NVS + Wi-Fi can coexist without playback.

## Full-memory diagnostic family
`UPCBadger_SD_PRODUCT_FULL_MEMORY_TEST_V3/V4/V5`
Useful evidence for player memory/order.

## SD-first diagnostic
`UPCBadger_SD_PRODUCT_FULL_MEMORY_TEST_V6_SD_FIRST.ino`
Explicitly delays the 96 KiB staging allocation until after SD mount.

## Product A/B V7
`UPCBadger_SD_PRODUCT_AB_TEST_V7_NO_WEB_SERVER.ino`
Explored WebServer/DNSServer contribution.
Do not treat the WebServer theory as proven root cause.

---

# NEXT STEP

The next revision should keep the entire proven V11 player/product flow intact and change only the staging capacity to the smallest evidence-backed value that the streaming logic can support.

Goal:

TFT
→ SD
→ CBP index
→ allocate smaller staging buffer
→ Xbox boot animation
→ NVS/config
→ Gamer ID/profile
→ background Wi-Fi

No changes to:
- TFT pins
- SD pins
- SPI buses
- SPI frequencies
- CS handling
- CBP format
- decoder algorithm
- DMA buffers
- boot graphics
- product flow

---

# VERSION DISCIPLINE

Current tested version:
`v1.14`

Next new change:
`v1.15`

Active filenames:
`UPCBadger_v1.15`

No descriptive suffixes for active versions.

Known-good versions remain protected.

---

# ZIP DISCIPLINE

Every Arduino test package:
- ZIP by default
- all required files included
- flat ZIP root
- no enclosing folder
- no nested folders
- actual ZIP contents double-checked before delivery

Rule:
ZIP BY DEFAULT → ALL REQUIRED FILES → FLAT STRUCTURE → DOUBLE CHECK.

---

# INSPECTOR GADGET

Evidence over experimentation.

Before a new test:
1. Search Library first.
2. Use actual Library source as source of truth.
3. Establish protected baseline.
4. Compare exact implementations.
5. Separate evidence / inference / speculation.
6. Reuse proven code.
7. Preserve all hardware/software details from proven paths.
8. Make the minimum controlled change.
9. Do not run unrelated changes together.
10. DOUBLE CHECK the final package.

Physical safety:
UNPLUG THE ESP32 BEFORE WIRING CHANGES OR SOLDERING.

---

# TESTING

For the next test:
- Arduino IDE 2.3.10
- compile
- upload
- Serial Monitor 115200
- send the complete log

Only the actual result matters; do not interpret it manually.
