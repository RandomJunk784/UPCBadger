# UPCBadger v2.34 Build Evidence — 2026-09-29

Base: v2.31 protected physical recovery point.

Reason for change:
v2.32 direct 208->127 and v2.33 direct 208->115 LovyanGFX scaled-sprite decodes both failed at the PNG decode stage.

v2.34 approach:
- recovered v1.85 static-Pngle decoder architecture
- existing two 32 KiB DMA buffers represented as a 64 KiB union
- Pngle placed in that workspace only during synchronous gamerpic decode
- two 208px RGB565 source rows retained
- one 127px RGB565 output row retained
- bilinear 208->127 resampling during decode
- circular 127px output
- existing R/B correction preserved

Validation:
- source delimiter sanity check passed
- existing 5-minute XBL presence path retained
- existing 20-minute profile sync retained
- no 424 source request
- no large gamerpic framebuffer
- Arduino compilation and physical flashing not claimed
