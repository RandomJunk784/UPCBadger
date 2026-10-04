# OGBadger — Original Xbox Jewel

Status: concept / feasibility development
Updated: 2026-10-04

## Product concept

A replacement/conversion Original Xbox jewel containing an ESP32-S3 and round LCD.

Primary goal: reproduce the Xbox boot experience visually while creating a platform that can become a live information/customisation display on modded consoles.

## Product strategy

Develop quietly and validate with a small number of real OG Xbox users before public launch.

Likely initial commercial model:
- Mail-in conversion: customer sends their original Xbox lid; Badger is fitted and returned.
- Exchange/pre-converted lid: customer receives a prepared lid for faster turnaround.
- DIY kit: later option for customers who want to install it themselves.

The first production approach should favour small flash-sale batches rather than large inventory.

## Stock Xbox mode

Target user experience:
1. Fit/receive converted lid.
2. Connect one inline power lead using the supplied DVD-drive power adapter.
3. Close the Xbox.
4. Power on.

No motherboard soldering and no Xbox data connection.

Expected behaviour:
- Badger powers from the Xbox.
- Factory-style Xbox boot animation.
- End on the Xbox X.
- Transition to a green X to recreate the original jewel.
- Subtle pixel shifting while idle to reduce static-pixel burn-in risk.

The stock product must not depend on Xbox telemetry, network access, or modification.

## Modded Xbox mode

Target concept:
- Run a Badger companion XBE on the modded Xbox.
- XBE reads available system information.
- Xbox communicates telemetry to the ESP32-S3 over the local network.
- Badger automatically discovers the Xbox; avoid manual IP/port configuration if technically practical.

Potential telemetry:
- CPU temperature
- GPU temperature
- motherboard temperature
- fan state/duty
- HDD/SSD information
- RAM
- motherboard revision
- BIOS information
- network information
- game/dashboard state where reliably obtainable

Potential display behaviour:
- Multiple scrolling screens
- System status pages
- Graphical animations
- Warnings
- Game/state-dependent displays

Important: telemetry capabilities and cross-revision compatibility remain to be researched and proven.

## Custom animation / creator mode

Potential future feature:
- User-created boot videos/animations.
- Define a strict Badger animation specification so AI-generated video can be converted reliably.

Candidate constraints to investigate:
- 360x360 square output
- approximately 15 FPS
- fixed maximum duration
- fixed maximum package size
- conversion performed on PC rather than requiring the ESP32 to decode H.264/HEVC/etc.

Possible pipeline:
AI video -> crop/resize -> frame-rate conversion -> RGB565 -> Badger compression/package -> Badger.

Do not assume custom animations must fit entirely in internal flash. Existing SD/CBP architecture may provide a better storage path. Protected factory fallback should always remain recoverable.

## Hardware direction

Display target is approximately 67.28 mm rendered image diameter. Physical hole size is not a constraint; artwork can be rendered to 67.28 mm.

Investigate round displays around 67-71 mm, including higher-resolution 480x480 panels.

Important distinction:
- Genuine SPI pixel-driven TFT is simplest because existing Badger display architecture is proven.
- ST7701 round panels using SPI for commands plus RGB parallel pixel data are a different architecture and require significantly more ESP32-S3 GPIO/bus resources.

Do not change Backpack Badger hardware assumptions merely to support OGBadger.

## Power

Preferred customer installation:
- Inline adapter at the Xbox DVD-drive power connection.
- Badger receives console power.
- No external power bank.
- No motherboard soldering for the stock product.

The DVD connector is being treated primarily as a power source. Do not assume it provides useful general Xbox telemetry.

## Xbox development target

A modded OG Xbox is being sourced for development, likely a 1.6 running Cerbios via onboard firmware modification.

Development target should be a reasonably standard hard-modded/modded Xbox with:
- working Ethernet
- ability to run arbitrary XBE software
- stable dashboard
- known motherboard revision
- known BIOS/mod environment

The 1.6/Cerbios machine is useful as a real-world development target. Do not assume capabilities without testing.

## Key architecture decision

Do not make the stock product dependent on electrical access to Xbox data buses.

The preferred split is:

STOCK:
Xbox power -> Badger -> boot animation -> X -> green X -> idle

MODDED:
Xbox power -> Badger
+
Xbox XBE -> network telemetry -> Badger
-> boot animation -> X -> live/custom screens

## Product moat / differentiation

The physical electronics are inherently copyable.

Differentiation should come from:
- polished OEM-like installation
- one-plug customer experience
- reliable firmware
- Xbox-specific XBE
- automatic discovery
- telemetry implementation
- animation format/converter
- creator ecosystem
- compatibility testing
- small-batch product quality

Do not claim exclusivity or uniqueness without further market research.

## Research queue

1. Exact round LCD options and current pricing.
2. Display interface requirements and GPIO impact.
3. Xbox lid/jewel compatibility across motherboard/revision/region variants.
4. DVD power adapter design and connector identification.
5. XBE hardware telemetry access across Xbox 1.0-1.6.
6. Cerbios capabilities relevant to telemetry.
7. Automatic Xbox/Badger network discovery.
8. Minimal Xbox-side Badger XBE.
9. Custom animation compression and CBP format limits.
10. Burn-in mitigation/pixel-shift strategy.
11. Existing competing Xbox jewel products and projects.
12. OG Xbox community/customer validation opportunities.
13. Microsoft/Xbox trademark and copyright considerations for commercial distribution.
14. BOM, conversion labour, postage and viable small-batch pricing.

## Evidence discipline

FACT, OBSERVATION, HYPOTHESIS and UNKNOWN must remain distinguishable.

Do not treat forum claims, seller descriptions, or untested XBE capabilities as proven.

Do not modify the protected Backpack Badger baseline to develop OGBadger.
