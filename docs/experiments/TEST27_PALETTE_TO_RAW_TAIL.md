# TEST 27 — Palette-to-RAW RGB565 Tail

Current experiment for the UPCBadger 330x350 fast base.

Schedule:
- frames 1–23: palette96 + LZ4 HC9
- frames 24–83: palette192 + LZ4 HC9
- frames 84–107: raw RGB565

The final 1.0 second therefore receives full RGB565 colour to eliminate the visible palette banding seen in TEST 23/25/26.

Asset:
ConsoleBadger_330x350_HYBRID_PALETTE_TO_RAW_RGB565_TAIL.CBP
Size: 11,040,787 bytes
SHA-256: 978b0151134131e457ee64d655f6fe58399ba862035699b72bfa4a0ce299daee

Firmware:
UPCBadger_TEST_27_330x350_HYBRID_PALETTE_TO_RAW_RGB565_TAIL_DIAGNOSTIC.ino
SHA-256: 9ed5f0649f1533fd6420f02e58e15e784faf14a9424514021dd1e3dc35800e7d

This experiment has not yet been physically benchmarked.