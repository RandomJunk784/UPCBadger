# UPCBadger v1.37 packaging correction — 2026-09-26

The first distributed v1.37 package contained an accidental source-assembly defect.

At the end of the opening source comment, these eight changelog lines were outside the comment block:

  v1.37:
    - Based directly on protected v1.36.
    - Removes the decorative top XBOX label...
    - ...

Arduino therefore interpreted `v1.37:` as C++ and reported:
`error: too many decimal points in number`

This was a packaging/source assembly error, not an intended firmware change.

Correction:
- removed the stray live-code changelog block
- regenerated `SHA256SUMS.txt`
- rebuilt the flat v1.37 ZIP
- re-ran ZIP integrity validation
- revalidated all package-internal SHA-256 values
- preserved the V27 CBP assets unchanged
- retained the defective package in the Library as evidence

Corrected v1.37 outer ZIP SHA-256:
`a698760971c0b6a81f04a31209e3c1c90e9e6f2e1b49fcef715c9f3cdf13c266`

The Library canonical package `UPCBadger_v1.37.zip` now points to the corrected build.

The next physical step remains compile/upload of the corrected v1.37 package.