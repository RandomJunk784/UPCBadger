# UPCBadger v2.19 — Orbitron Typography

## BASE
v2.18.

## FONT SOURCE
LovyanGFX built-in:
`fonts::Orbitron_Light_24`

LovyanGFX supports fractional `setTextSize()` scaling, so the supplied
24px Orbitron font is rendered at the requested approximate pixel sizes.

## PROFILE FONT MAP

GamerTag:
`24px * (16/24) = ~16px`

GamerScore:
`24px * (16/24) = ~16px`

ONLINE:
`24px * (12/24) = ~12px`

WiFi:
`24px * (12/24) = ~12px`

## CHANGES
Only profile typography changed.

- GamerTag -> Orbitron ~16px
- GamerScore -> Orbitron ~16px
- ONLINE -> Orbitron ~12px
- WiFi -> Orbitron ~12px
- Text-size state is reset to 1.0 after every Orbitron draw.
- GamerTag width check is performed using the actual Orbitron scale.
- GAMERSCORE label remains removed.

## UNCHANGED
- gamerpic renderer
- XBL image source/download
- proven R/B correction
- circular gamerpic/ring
- lime colours
- Xbox orb
- profile geometry
- animation protection
- CBP animation
- Wi-Fi
- TLS
- XBL
- SD
- staging
- permanent crash/heap diagnostics

## STATUS
UNFLASHED.

No Arduino IDE compile claim is made here.