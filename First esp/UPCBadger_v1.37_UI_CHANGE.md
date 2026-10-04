# UPCBadger v1.37 — UI Change Record

Base: protected v1.36

Profile overlay changes only:
1. Remove top decorative "XBOX" label.
2. Remove "GAMERSCORE" label and "--" placeholder.
3. Replace old lower status row with:
   - white ONLINE indicator + ONLINE text
   - white Wi-Fi icon + Wi-Fi text
4. Position the two status groups together and evenly spaced around the lower centre.
5. Keep Wi-Fi connectivity logic unchanged.
6. Keep V27 BOOTGREEN_EXACT CBP assets unchanged.
7. No change to boot animation, CBP playback/decoder, NVS, pinout or SPI.

Validation:
- source braces balanced
- requested overlay strings removed/present as expected
- package ZIP integrity checked
- physical LCD test still pending
