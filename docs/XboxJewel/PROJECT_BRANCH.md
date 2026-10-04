# OG Xbox Video Jewel — Project Branch

Status: CONCEPT / DEVELOPMENT BRANCH
Created: 2026-10-04
Base: BPBadger

## Purpose

Develop a purpose-built animated replacement Xbox jewel using the Badger platform.

This branch is separate from the Backpack Badger production work. Do not alter the protected Backpack Badger baseline or assume Xbox Jewel requirements belong in the production Backpack PCB.

## Product concept

A replacement/modification service for the original Xbox jewel/lid.

Primary goal:

> Fit the lid, connect one plug, and reproduce the Xbox boot experience without requiring motherboard soldering.

Initial commercial model:
- Mail-in conversion: customer sends their original Xbox lid; Badger is fitted and the lid is returned.
- Exchange conversion: pre-converted used original Xbox lid supplied for faster turnaround; cosmetic wear/scratches to be disclosed.
- DIY kit may follow once the design is proven.

## Stock Xbox mode

No Xbox data connection required.

Sequence:
1. Xbox powers Badger through an inline DVD-drive power adapter.
2. Badger plays the factory-style Xbox boot animation.
3. Animation ends on the Xbox X.
4. Display transitions to the green X jewel state.
5. Pixel shifting is used during static/idle display to reduce burn-in risk.

Target installation:
- One internal plug/power connection.
- No soldering for customer.
- No external USB cable required.
- Reversible installation preferred.

## Modded Xbox mode

Target architecture:
- Xbox runs a purpose-built Badger XBE.
- XBE reads available Xbox system information.
- XBE communicates with the Badger over the local network.
- Badger automatically discovers/pairs with the Xbox where practical.
- Avoid manual IP/port configuration for normal users.

Potential telemetry:
- CPU/GPU information
- CPU/GPU/system temperatures where reliably accessible
- fan information
- HDD/SSD information
- RAM
- motherboard revision
- BIOS/mod environment
- network information
- game/dashboard state where obtainable

Potential UI:
- multiple scrolling screens
- system status pages
- graphical animations
- warnings
- game-specific or event-driven displays

Important: XbDiag demonstrates substantial OG Xbox hardware/system information can be accessed from Xbox-side software, but our own XBE and protocol are preferred. Exact data availability and compatibility across Xbox revisions must be tested.

## User experience target

Normal customer:
1. Fit replacement/converted lid.
2. Connect the single supplied power lead.
3. For modded mode, install/run the Badger XBE once.
4. No manual networking configuration if automatic discovery/provisioning can be achieved.

The intended end state is effectively:
> Fit lid → one plug → run/install once → done.

## Custom animation / Creator mode

Potential future feature.

User supplies a custom square boot video. Badger software converts it into the internal Badger animation format.

Candidate specification to investigate:
- 360×360 source/output
- 15 FPS
- short fixed maximum duration, initially around 8 seconds
- strict maximum package/file size
- conversion performed before the file reaches the ESP32

The format must be optimised for ESP32-S3 playback rather than requiring H.264/HEVC/AV1 decoding.

Factory animation must remain recoverable/protected.

Storage options:
- internal flash for firmware/fallback assets
- microSD for larger/custom assets if retained in the hardware
- determine whether compressed custom animations can realistically fit within the N16 flash budget

## Hardware direction

Current display target:
- round LCD around 67–71 mm physical diameter
- visible rendered circle can be constrained to Ø67.28 mm
- 480×480 is attractive as a next-generation target

A ~70.13 mm 480×480 ST7701 panel is a candidate, but many ST7701 panels use SPI for commands plus RGB parallel pixel transfer rather than SPI pixel transfer. This is a new display architecture and must not be assumed compatible with the existing GC9B72 SPI implementation.

Preferred outcome:
- genuine 480×480 round SPI display if a suitable panel exists.

Fallback:
- ESP32-S3 RGB/LCD peripheral driving an ST7701-based panel, with PSRAM as appropriate.

Current known Badger experience with GC9B72/ESP32-S3 remains useful but is not automatically transferable to ST7701 RGB panels.

## Xbox hardware/power

Stock Xbox:
- Do not rely on motherboard data taps.
- Do not require customer soldering.
- Inline DVD-drive power adapter is the preferred power path.
- DVD connector is considered a power source, not a general telemetry interface.

Modded Xbox:
- Network communication is preferred over electrical tapping.
- Xbox motherboard SMBus/LPC access is not part of the normal consumer installation.

Development Xbox:
- A modded OG Xbox is being obtained for development.
- Expected machine: Xbox 1.6 with Cerbios/onboard firmware modification, supplied by an experienced Xbox modder.
- Use it as a real development target, not as proof that all Xbox revisions behave identically.
- Later test compatibility across earlier revisions where practical.

## Product strategy

Do not assume the physical hardware is defensible by itself.

Potential product family:
- Classic/Stock: authentic animated jewel experience.
- Smart/Modded: live Xbox telemetry and multiple screens.
- Creator: user-generated/custom boot animations.

Possible commercial approach:
- small flash-sale production drops
- first run around 10 units after private testing
- avoid large inventory commitment until demand is demonstrated
- quietly test with knowledgeable OG Xbox users before public launch

Potential moat:
- polished installation
- tested compatibility
- Badger firmware
- purpose-built Xbox XBE
- automatic discovery
- custom animation format/converter
- telemetry implementation
- ongoing software features

## Development principle

Build the smallest compelling product first:

> Power on → excellent Xbox boot animation → X → green X.

Do not build the full modded/creator ecosystem before proving that the basic jewel makes an OG Xbox owner want one.

## Research / unknowns

1. Exact 480×480 round display options, interfaces and availability.
2. Best production display architecture for ESP32-S3.
3. Universal/reliable power adapter approach across Xbox revisions.
4. Exact XBE hardware data available on Xbox 1.0–1.6.
5. Reliable Xbox-to-Badger network protocol.
6. Automatic discovery with zero manual networking.
7. Whether an Xbox-side XBE can persist/start automatically in common mod environments.
8. Custom animation compression and practical flash/storage limits.
9. Burn-in mitigation/pixel-shift strategy.
10. Physical lid/jewel revision compatibility.
11. Mail-in/exchange refurbishment workflow.
12. Existing competing products and projects.
13. IP/trademark/copyright considerations around the Xbox visual identity and factory-style animation.
14. Community demand and realistic selling price.

## Evidence discipline

FACT, OBSERVATION, HYPOTHESIS and UNKNOWN must remain clearly distinguished during development.

Do not claim compatibility, performance, Xbox telemetry access, automatic discovery, or commercial demand until tested or supported by evidence.

Do not copy assumptions from older Backpack Badger versions into this branch without checking them.

## Branch boundary

This branch is for the OG Xbox Video Jewel concept.

Do not modify protected Backpack Badger baselines from this branch.
