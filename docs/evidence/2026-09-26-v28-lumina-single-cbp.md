# UPCBadger V28 / v1.38 — Lumina Single-CBP Baseline

Date: 2026-09-26

## Current status

v1.38 is a prepared PC-verified test build. It has NOT yet been compiled/uploaded to the ESP32 or physically verified.

Latest physically tested firmware remains protected v1.36.

## New source

User-supplied:
2026-09-26_11-08-26_Lumina.mp4

Measured:
- 640x640
- 24 FPS
- 97 frames
- 4.041667 s source duration

## V28 asset

Runtime filename:
profile_energy.CBP

Properties:
- 360x360
- 97 frames
- 12 FPS
- 8.083333 s loop
- continuous playback
- one full-screen CBP1 asset
- no profile_energy_patch.CBP required by v1.38

CBP SHA-256:
1f12cd6cad81253bd32cf9ac76065b7895af842c86d249c69680787de93cd025

## Colour method

- Fresh palette generated from the new Lumina animation itself.
- One stable 256-entry RGB565 palette across the whole animation.
- RGB565-aware weighted K-means palette optimisation.
- No dithering.
- Exact black retained as an anchor.
- Source Lumina colours are not remapped into the old ConsoleBadger/V27 palette.
- The visible Lumina AI watermark was removed with a small local inpaint mask before quantisation; this is the only deliberate source-image cleanup.

PC round-trip quality:
- aggregate RMSE ~1.71 RGB levels/channel in expanded RGB565 display space
- representative frame RMSE ~1.88 (frame 0), ~1.72 (frame 48), ~1.79 (frame 96)

## Why one stable palette

A single animation-wide palette avoids temporal palette crawling/flicker that can appear when each frame receives a completely unrelated adaptive palette.

The source contains only about 2,800 distinct RGB565 colours across the full clip, so a carefully optimised 256-colour shared palette is sufficient for a clean result.

## Playback

12 FPS is intentional:
- existing ConsoleBadger boot is already visually smooth at 12 FPS on the real GC9B72
- all 97 Lumina frames are retained
- no frame duplication, dropping or synthetic interpolation
- continuous 8.08 s looping provides ongoing visual motion for the small TFT

The v1.38 scheduler is deadline-oriented and avoids cumulative frame-delay drift. If a frame overruns, the schedule recovers to a future deadline rather than bursting several frames.

## Architecture

Previous architecture:
profile_energy.CBP + profile_energy_patch.CBP + foreground compositor.

V28:
ONE full-screen profile_energy.CBP + live firmware overlay.

The patch layer is removed from v1.38 runtime use.

The live overlay remains separate so future Xbox API data can supply gamerpic, gamertag and status without regenerating the Lumina background.

## Known limitation for first physical test

Because the new background currently writes the full screen, v1.38 redraws the live overlay after each background frame.

If physical testing shows overlay flicker, the next change should be a protected overlay/no-write region or small overlay compositor. The Lumina asset itself should NOT be recoloured or regenerated solely to address an overlay rendering issue.

## Package

UPCBadger_v1.38.zip

Outer SHA-256:
9cd7491e77029e080ce4aca4ed9e5a352f10e8ae247b864d16cb84339b053d06

Package contents:
- README.txt
- SHA256SUMS.txt
- UPCBadger_v1.38.ino
- profile_energy.CBP

## Toolchain / hardware lock

- ESP32 DOIT DevKit 1
- GC9B72 360x360
- TFT SPI 80 MHz
- SD SPI request 40 MHz
- LovyanGFX 1.2.29
- Arduino IDE 2.3.10
- ESP32 package 3.3.11
- TFT: MOSI 23, SCLK 18, CS 21, DC 22, RST 4
- SD: MOSI 27, SCLK 25, MISO 26, CS 13

## Evidence stored in Library

- original Lumina source video
- V28 decoded 12 FPS preview
- V28 contact sheet
- source-vs-CBP frame comparison
- V28 global palette file
- V28/v1.38 engineering handoff
- v1.38 package
- checksums

Before wiring/soldering:
UNPLUG THE ESP32 FIRST.
