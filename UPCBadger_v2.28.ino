// V2.03: restore the last physically successful V1.96 RGB-order gamerpic method
// through LovyanGFX's panel API, while eliminating the V2.00-V2.02 raw MADCTL mirror regression.
/*
  UPCBadger v2.27
  v2.17:
    - Refines the final profile presentation only.
    - Gamerpic ring changed to the ConsoleBadger lime-green family.
    - Uses a soft 3-level lime ring edge to reduce visible circle jaggies.
    - GamerTag underline uses the same lime-green colour.
    - Removes the GAMERSCORE text label.
    - Tightens the GamerScore protected region to give more animation space.
    - Permanent crash/heap diagnostics from v2.16 remain unchanged.
    - No changes to XBL, Wi-Fi, TLS, SD, CBP, gamerpic decode, R/B correction,
      scaling, orientation or network/memory behaviour.

  v2.18:
    - Locks profile accent green to RGB(120,255,40), sampled from the saved target.
    - Uses the same exact lime for the GamerTag underline and gamerpic ring core.
    - Reworks the gamerpic ring into a smoother pixel-level lime edge.
    - Replaces the crude Xbox score icon with a layered premium-style orb.
    - Makes GamerTag and GamerScore text bold via controlled multi-pass rendering.
    - Rechecks and expands XBL overlay protection boxes only where bold geometry requires it.
    - Removes the final GAMERSCORE label already requested in v2.17.
    - Permanent crash/heap diagnostics remain unchanged.
    - No changes to XBL, Wi-Fi, TLS, SD, CBP, PNG decode, R/B correction or orientation.

  v2.21:
    - Moves the NORTH GamerTag/status assembly closer to the centre gamerpic.
    - Moves the SOUTH GamerScore closer to the centre gamerpic.
    - Changes the GamerTag status bar and gamerpic ring to exact #C7FF55
      (RGB 199,255,85).
    - Tightens the invisible animation-protection footprints around the
      GamerTag/status and GamerScore so the background field reads more freely.
    - No gamerpic source/download/decoder/upscale changes.
    - No XBL, Wi-Fi, TLS, SD, CBP animation engine or memory-recovery changes.

  v2.22:
    - TEST ONLY: changes the GamerTag status bar and gamerpic ring to exact
      #9AF52A (RGB 154,245,42) to empirically test the desired display colour.
    - No geometry, animation, Wi-Fi, XBL, TLS, SD or gamerpic source changes.

  v2.25:
    - Keeps the profile animation at 12 FPS with no per-frame text/status draw calls.
    - XBL profile data refresh is changed to every 20 minutes.
    - Wi-Fi and XBL status-bar state are refreshed on a 60-second service cadence.
    - Static GamerTag and GamerScore are rendered once and retained as exact pixel masks;
      animation frames composite those pixels in-stream instead of redrawing the text.
    - Status-bar pixels are composited in-stream from the cached one-minute status state.
    - No rectangular frozen profile fields are used.
    - No gamerpic source/quality changes, no Wi-Fi transport changes, no TLS transport
      changes, no SD/CBP format changes, and no animation timing changes.

  v2.27:
    - Moves the 360px static-overlay row buffer from global DRAM .bss to the
      frame-render function stack. This removes 720 bytes of static DRAM usage
      without changing the compositor, animation timing or network behaviour.
    - No runtime/product behaviour changes intended.

  v2.26:
    - Compile-fix release only: profileOverlayRow uses the fixed 360px animation width
      because PROFILE_ANIM_WIDTH is declared later in the source.
    - No runtime/product behaviour changes from v2.25.

  v2.24:
    - Removes the external green gamerpic presentation ring completely.
    - The gamerpic remains a 115px circular image; only the actual image pixels
      are rendered and protected during animation playback.
    - Removes the former ring coverage renderer and its RGB565 colour work.
    - Tightens circular animation protection from the old outer ring radius to
      the actual 57px gamerpic image radius so no invisible halo is frozen.
    - No changes to gamerpic source/download/decoder, R/B correction or scaling.
    - No changes to GamerTag/status/score layout, animation timing, Wi-Fi, XBL,
      TLS, SD, CBP format or memory-recovery behaviour.

  v2.23:
    - Removes rectangular animation protection from GamerTag/status and GamerScore.
    - Normal profile animation now protects ONLY the circular gamerpic.
    - GamerTag, split status bar and GamerScore are redrawn transparently after each
      animation frame so the moving background remains visible underneath them.
    - Keeps the existing #9AF52A colour test isolated; no colour change in this revision.
    - No gamerpic source/quality, Wi-Fi, XBL, TLS, SD, CBP format or geometry changes.

  v2.20:
    - Uses screenshot-reference lime RGB(216,248,40) (#D8F828) for the live status bar.
    - Uses the same screenshot-reference lime for the gamerpic ring.
    - Keeps the v2.19 Orbitron GamerTag treatment at ~16px.
    - Replaces the GamerTag underline with a two-part live status bar.
    - Status bar matches the rendered GamerTag width exactly, is 2px thick,
      and has a small centre break.
    - Left status segment = XBL presence: lime when confirmed ONLINE, white otherwise.
    - Right status segment = home Wi-Fi: lime when CONNECTED, white otherwise.
    - Removes the separate ONLINE beacon/text and Wi-Fi icon/text.
    - Removes the Xbox score orb.
    - GamerScore changes from ~16px Orbitron to 24px Orbitron, centred below the gamerpic.
    - Enlarges only the static GamerTag/status and GamerScore protection footprints
      required by the new geometry.
    - No changes to gamerpic decode, R/B correction, ring, XBL transport, Wi-Fi
      transport, TLS, SD, CBP animation engine or memory-recovery sequence.

  v2.16:
    - Refined final-profile candidate built directly from the physical v2.07-known-good baseline.
    - Preserves the proven 208x208 PNG -> 72x72 decode and exact R/B correction.
    - Software-upscales the corrected 72x72 image to a 115px circular gamerpic.
    - Adds a thin green 119px presentation ring.
    - Uses circle-aware animation protection so CBP remains visible outside the gamerpic.
    - Tunes the profile geometry to the saved target reference.
    - Corrects the software swap565 packing to match LovyanGFX's published swap565_t layout.
    - No XBL, TLS, Wi-Fi, SD, CBP, MADCTL, inversion or rotation changes.

  v2.14:
    - CLEAN BASELINE RESTORE from the physically working v2.07 source.
    - No gamerpic renderer changes.
    - No memory changes.
    - No Wi-Fi, TLS, XBL, SD/CBP or animation changes.
    - No display/MADCTL/rotation/colour-mode changes.
    - No UI geometry or beacon changes.
    - Only the visible firmware revision banner/changelog identity is updated.

  v2.07:
    - Fixes V2.06 link-time DRAM overflow by moving the 72x72 gamerpic buffer from static BSS to temporary heap allocation.
    - Corrects the V2.06 RGB565 R/B swap mask to match LovyanGFX 1.2.29 swap565_t:
      R = bits 3..7, B = bits 8..12.
    - Keeps the V2.03 mirror-safe panel state: no raw MADCTL, no RGB/BGR mode changes, no inversion.
    - Software-only gamerpic R/B correction.
  v2.07:
    - Removes all raw MADCTL writes from the gamerpic renderer.
    - Fixes the global left/right mirror caused by V2.00-V2.02 forcing MX in MADCTL.
    - Uses tft.getPanel()->config() + tft.setRotation() to reproduce the V1.96
      temporary RGB-order mode without changing rotation bits.
    - Temporary gamerpic state is RGB order, rotation preserved; normal product
      state is restored through LovyanGFX after the draw.
    - No Wi-Fi, TLS, XBL, SD/CBP, animation, UI geometry or cache changes.
    - Keeps the established 208x208 -> 72x72 PNG gamerpic path and PNG memory release.

  v2.00:
    - Based directly on the physically working v1.96.
    - Gamerpic colour-only correction: changes only the GC9B72 MADCTL BGR bit
      during PNG rendering, preserving the required MX orientation bit.
    - Uses MADCTL 0x40 for gamerpic RGB and restores 0x48 for product BGR.
    - Does NOT alter LovyanGFX panel configuration or rotation.
    - Does NOT touch Wi-Fi, TLS, XBL, staging memory, SD/CBP playback,
      animation timing, UI geometry or gamerpic scaling.
    - Specifically avoids the earlier 0x00/0x08 experiment which removed
      MX and caused the entire gamerpic to mirror left-to-right.

  v1.96:
    - Based directly on the physically successful V1.94/V1.95 profile-refresh baseline.
    - Gamerpic-only colour/crop test: explicitly scales the 208x208 Xbox PNG to the
      72x72 gamerpic window and temporarily switches the panel to RGB mode only
      during PNG rendering, then restores the original product BGR mode.
    - Retains the V1.95 live Gamer ID correction: the stale configured Gamer ID is
      erased during live refresh while the already-drawn gamerpic remains protected.
    - No Wi-Fi, TLS, XBL transport, animation frames, timing, UI geometry, SD player
      or boot animation changes.

  v1.95:
    - Gamer ID carry-forward correction: live profile refresh protects only the gamerpic
      region so the configured fallback Gamer ID is erased and replaced by the live tag.

  v1.94:
  CHANGELOG
  ----------
  v1.94:
    - Based directly on physically tested V1.93.
    - Preserves the already-drawn startup gamerpic during the first live XBL profile refresh.
    - Live refresh redraws the animation frame WITH all static profile regions protected, then redraws live text/status only.
    - Prevents the second low-memory PNG decode that was failing after TLS/staging restoration.
    - No Wi-Fi, TLS, XBL transport, gamerpic renderer, UI geometry, animation frames, timing, TFT or SD changes.

  v1.93:
    - Based on the physically tested V1.88/V1.90 gamerpic path.
    - Keeps the exact V1.61 GamerPicFileWrapper + drawCachedGamerPic() path.
    - Releases LovyanGFX 1.2.29 persistent PNG decoder memory immediately
      after each gamerpic draw using tft.releasePngMemory().
    - This prevents the retained pngle decoder workspace from starving the
      later Wi-Fi driver initialisation.
    - Wi-Fi startup remains after the initial gamerpic draw.
    - No RX-buffer count changes.
    - No staging-buffer release/reallocation for Wi-Fi startup.
    - No UI geometry, fonts, animation frames, animation timing, display
      pins, SD pins, XBL data handling or profile artwork changes.

  v1.90:
    - Wi-Fi-only repair based directly on the physically tested V1.89.
    - setup() now calls the existing V1.61-proven startHomeWiFiBackground()
      helper instead of the lingering V1.65 clean-driver shutdown/WIFI_OFF
      sequence that V1.89 was still executing directly in setup().
    - Connection retries now call WiFi.begin() only; they do not call
      WiFi.disconnect() or re-enter WiFi.mode(), avoiding repeated Wi-Fi
      driver reinitialisation and RX-buffer setup attempts.
    - No gamerpic renderer/drawing, UI geometry, animation, TFT, SD or XBL
      changes.

  v1.88:
    - One-line gamerpic startup guard restoration based on V1.61; no UI or animation changes.
    - Surgical recovery build based on the V1.81-era live-profile source.
    - Restores the exact V1.61 GamerPicFileWrapper + drawCachedGamerPic()
      implementation.
    - Restores the V1.61 startup ordering: initial profile/gamerpic render
      completes before background home Wi-Fi starts.
    - No UI geometry, fonts, animation frames, animation timing, display
      pins, SD pins, XBL data handling, or profile artwork changes.
    - The existing later Wi-Fi driver handling remains intact; only its
      startup point is moved until after the initial gamerpic render.

  v1.65:
    - Wi-Fi forensic/fix build based directly on the physically tested V1.64.
    - Profile identity and gamerpic behaviour preserved unchanged.
    - Background Wi-Fi now explicitly shuts down any existing Wi-Fi driver/mode
      before entering clean station mode.
    - Added Wi-Fi driver-state diagnostics around the transition.
    - No manual RX-buffer configuration is introduced; the Arduino/ESP32 Wi-Fi
      stack retains ownership of its buffer configuration.
    - Serial product banner corrected from the stale V1.50 text to V1.65.
    - No changes to XBL lookup, TLS transport, gamerpic cache, profile layout,
      GamerTag fallback, GamerScore demo, beacon, animation or SD player.

  v1.64:
    - Based on the V1.61 known-good source with the V1.63 profile-layout
      decisions carried forward.
    - WEST Xbox Live status is a compact green beacon only; no ONLINE/OFFLINE
      text and no oversized protected field.
    - NORTH GamerTag protected field tightened further to reduce animation
      intrusion while retaining a practical 12-character baseline.
    - SOUTH GamerScore is a single-line field sized for an 8-digit value.
    - The V1.63 10,000,000 GamerScore demo remains enabled for physical sizing.
    - No gamerpic bezel, ring or circle is drawn. Gamerpic remains square.
    - Configured GamerTag is now immediately used as the displayed fallback
      identity from NVS. A successful XBL lookup replaces it with the live
      GamerTag. A failed/absent XBL update leaves the configured/last-known
      name displayed; PLAYER is only an absolute empty-state fallback.
    - No changes to XBL transport, lookup target, gamerpic download/cache,
      presence logic, Wi-Fi scheduler, TLS/memory recovery, animation timing,
      SD/CBP player or hardware pinout.

  v1.61:
    - Based directly on the physically tested v1.59 source.
    - Fixes the identity-flow bug: the GamerTag saved by the setup portal
      is now the actual OpenXBL lookup target. /v2/account is no longer used
      as the displayed-profile source, so changing the portal GamerTag changes
      the target profile rather than continuing to display the API-key owner.
    - Uses OpenXBL GamerTag search first, with player/friends-search fallbacks.
    - Reads live GamerTag, GamerScore, XUID, gamerpic URL and presence from
      the target profile response where supplied; XUID-specific presence is
      used only when the search response does not contain presence state.
    - Presence now has three states: ONLINE, OFFLINE, UNKNOWN. A transport
      failure can never falsely become OFFLINE.
    - Gamerpic download follows HTTP redirects and only re-fetches when the
      live image URL changes or the SD cache is absent.
    - East/Wi-Fi, West/presence and South/GamerScore display positions move
      approximately 5 mm inward while the protected rectangles are tightened.
    - North/GamerTag, centre/gamerpic, 12 FPS animation, SD/CBP player,
      32 KiB staging, TLS suspension/recovery, NVS portal and Wi-Fi scheduler
      architecture remain otherwise unchanged.
    - First live sync remains immediate after Wi-Fi connection; subsequent
      XBL profile refresh remains every 30 minutes.

  v1.59:
    - Based directly on V1.58.
    - First live XBL profile sync is now armed as soon as the product profile
      screen is activated, so the existing service loop performs the first
      sync immediately after Wi-Fi reaches CONNECTED and profile memory can
      be safely suspended/released.
    - Recurring live XBL profile refresh changed from 60 minutes to 30 minutes.
    - Wi-Fi indicator remains on the existing 30-second refresh cycle.
    - No change to the TLS/memory recovery ordering, gamerpic DataWrapper
      path, compass layout, animation timing, portal/NVS lookup key, or API
      transport.

  v1.55:
    - Compile-correction and hardening pass over v1.54.
    - Removes duplicate drawProfileWifiIcon definition.
    - Uses existing extractJsonUInt32 helper for live GamerScore parsing.
    - Replaces unsupported drawPngUrl() with an SD-cached gamerpic path using the existing LovyanGFX file decoder.
    - Gamerpic is downloaded only during the hourly live sync and drawn only when the live profile is initially populated or changes.
    - Keeps the V1.53 compass layout and 30-second Wi-Fi refresh architecture.
    - No changes to proven TLS/memory recovery.

  v1.54:
    - Based directly on the physically tested v1.53.
    - Keeps the proven OpenXBL account transport and TLS memory-recovery path unchanged.
    - Portal/NVS Gamer ID remains the XBL lookup key only. The displayed GamerTag comes from the successful XBL response and remains static until the next hourly XBL check.
    - Adds live GamerScore from the /v2/account Gamerscore setting.
    - Adds live Xbox presence from the documented OpenXBL /api/v2/presence endpoint; Online/Offline is refreshed on the same hourly XBL cycle.
    - Adds the live Xbox gamerpic from GameDisplayPicRaw. LovyanGFX renders the returned PNG URL once after a successful account sync; it is never touched by the 12 FPS animation loop.
    - Profile compass layout: GamerTag NORTH, Wi-Fi EAST, GamerScore SOUTH, Xbox Live presence WEST, gamerpic CENTRE.
    - Static overlay protection now covers the five actual UI regions only; no full-width bands.
    - Wi-Fi refresh remains every 30 seconds and redraws only the Wi-Fi region while preserving the other four static regions.
    - GamerTag, GamerScore, presence and gamerpic are redrawn only when the hourly live data changes or when the initial live sync completes.
    - A clean current-frame refresh is used when a static region genuinely changes, preventing stale pixels without reintroducing per-frame overlay redraw.
    - No change to boot animation, CBP format, SD bus, TFT pins, animation timing, portal/NVS mechanism or proven TLS recovery.

  v1.51:
    - Based directly on the physically tested v1.50.
    - Keeps the proven OpenXBL DNS/TCP/TLS/HTTP and memory-recovery path unchanged.
    - Parses the successful /v2/account response for the Xbox Gamertag.
    - Stores the returned Gamertag in liveGamerID and immediately redraws the
      live profile overlay so the API result becomes visible on the Badger.
    - Removes the duplicate transport PASS message from syncProfileNow(); the
      service layer remains the single owner of the final transport result line.

  v1.50:
    - Based directly on v1.48/v1.49 transport work.
    - Keeps the proven OpenXBL DNS/TCP/TLS/HTTP path unchanged.
    - Fixes TLS memory recovery ordering: the profile CBP file and 32 KiB
      animation staging buffer are suspended before the network transaction,
      but staging is restored only after syncProfileNow() has returned, so
      all HTTP/TLS local objects have been destroyed before malloc() runs.
    - Reopens and revalidates the master profile CBP after staging recovery.
    - Removes the temporary ONLINE dot/text from the profile screen; the
      live Wi-Fi indicator is now the only status indicator retained.
    - No changes to TFT geometry, animation timing, CBP format, boot player,
      NVS, Wi-Fi connection mechanism or API endpoint.

  v1.40:
    - Based directly on the physically tested v1.39.
    - Gamer ID moved lower into the black/gloss region and reduced to Font2;
      falls back to Font0 for unusually long usernames to protect the side edges.
    - Removed the visible Wi-Fi text; the white Wi-Fi beacon remains as the
      live visual placement/status indicator.
    - Moved ONLINE text/dot slightly farther outward to free central space.
    - V28 Lumina profile_energy.CBP is unchanged byte-for-byte except for
      cyclic frame ordering: the loop boundary is moved to the low-motion
      f12->f13 phase so the wrap occurs at a more symmetrical 3/9-o'clock
      energy presentation. No frame pixels or palette entries are altered.
    - Removed the old black-screen/brightness/2-pixel-shift retention cycle.
      Continuous Lumina motion is now the primary screen-retention measure;
      no forced five-minute black-screen refresh is performed.

  v1.46:
    - Based directly on v1.41 master-animation architecture.
    - At first-time setup, render the Gamer ID loop-start frame and preserve
      that frame while the existing setup AP/captive portal runs.
    - Configured units continue directly into the Gamer ID loop at the same
      master-frame boundary.
    - Manual config-button entry keeps the existing setup screen behaviour.

  v1.41:
    - One master 360x360 CBP1 animation asset supplies both boot and Gamer ID.
    - Boot frames 0..103 play once.
    - Gamer ID loop frames 104..180 repeat continuously.
    - NVS/setup/Wi-Fi/product services are retained.
    - Removes the separate profile animation asset from runtime use.

  v1.39:
  v1.18:
    - Visual profile UI revision only; v1.17 remains protected.
    - Added /profile_bg.pbg: 360x360 256-colour palette + LZ4 background.
    - Background is streamed in 40-row blocks using existing 32 KiB staging
      and one existing 32 KiB DMA buffer; no full-frame RAM is allocated.
    - Existing boot, SD, NVS, configuration and Wi-Fi product flow is unchanged.
    - Profile text remains live firmware overlays; avatar is still initials
      until live gamerpic retrieval is attached.
    - Added small Wi-Fi state icon in the lower-right corner.

  v1.16:
    - Based directly on v1.15 (keeps its diagnostic logging).
    - ONE functional change: STAGE_BUFFER_BYTES reduced from 96 KiB
      to 32 KiB, via a single named constant now used everywhere the
      old 96 KiB literal was hardcoded (stageFill, stageEnsure, the
      two beginPos compaction heuristics, and the malloc call).
    - Added a compile-time static_assert that STAGE_BUFFER_BYTES can
      never be smaller than one CBP block (8-byte CBPBlockHeader +
      BLOCK_INDEX_BYTES = 16392 bytes), so this can't silently be
      shrunk into a memory-corruption bug later.
    - Why: the decoder never needs more than 16392 contiguous bytes
      staged at once (verified directly against CBPBlockHeader,
      BLOCK_INDEX_BYTES and the stageEnsure() call sites in this
      source - not inferred). The 96 KiB buffer was a generous
      read-ahead reservoir, not a hard requirement. v1.14/v1.15
      showed SD init succeeding with a healthy 206644 bytes free
      total heap, yet the 96 KiB staging malloc still failing - a
      fragmentation signature, consistent with the existing v1.09
      Library evidence ("Largest block after SD: 65524", i.e.
      already less than 96 KiB but comfortably more than 32 KiB).
    - v1.15 was packaged as a pure diagnostic (no functional change)
      but was superseded by v1.16 before a physical test was run,
      once the 16392-byte real requirement was confirmed directly in
      this source - re-running the unmodified 96 KiB build only to
      log a number we could already derive from the code didn't meet
      the "smallest test that closes a real evidence gap" bar.
      v1.16 keeps v1.15's logging so the actual largest-free-block
      numbers still get recorded either way.

  v1.14:
    - Based directly on the Library's PRODUCT_MODE_V11 known-good source.
    - ONE functional change: the 96 KiB staging buffer is allocated only
      after SD initialisation and CBP index validation succeed.
    - This preserves the V11 TFT, SD, CBP decoder, DMA buffers, boot
      animation, profile, NVS, configuration and Wi-Fi implementation.
    - Purpose: test whether the 96 KiB staging allocation is the cause
      of the SD initialisation failure seen with the integrated player.
    - RESULT (2026-09-24): SD initialisation and CBP index load both
      succeeded (206644 bytes free heap before SD init). The 96 KiB
      staging malloc AFTER SD init failed. Treated as PARTIAL SUCCESS
      per the decision criteria: SD ordering problem solved, playback
      allocation stage now the open question.

  SAFETY:
    UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.

  v1.17:
    - Based directly on the tested v1.16 source.
    - ONE functional change: seek to indexTable[frame].offset before
      each call to playFramePipelined().
    - Evidence: the Library's proven playback paths explicitly seek
      to the indexed frame offset before calling playFramePipelined().
    - v1.16 successfully initialised SD, loaded the CBP index, allocated
      32 KiB staging, then failed at frame 0 with "invalid CBP block".
      The failure is consistent with reading the CBP header instead of
      frame 0 data because playXboxAnimation() opened a fresh file without
      seeking to the frame's indexed offset.
    - No change to staging size, decoder, DMA, TFT, SD, NVS, Wi-Fi,
      configuration, profile, pins or SPI frequencies.


  v1.19:
    - Based directly on the tested v1.18.
    - Keeps the profile background asset and all boot/SD/NVS/Wi-Fi
      behaviour unchanged.
    - Removes the hard-coded demo GamerScore value.
    - Removes the incorrect use of home Wi-Fi connection as Xbox ONLINE status.
    - Adds API-ready placeholder values for GamerScore and Xbox presence.
    - Keeps the saved Gamer ID as the current user-provided identity value.
    - Keeps gamer initials as the temporary gamerpic placeholder.
    - Keeps the small live home-Wi-Fi icon in the lower-right corner.
    - No profile background colour or layout change in this revision.


  v1.20:
    - Based directly on v1.19.
    - Profile UI layout only.
    - Removed the PROFILE title from the old second line.
    - Moved the saved Gamer ID into the former PROFILE-title position.
    - Re-centred the black gamer identity circle over the centre of the
      supplied profile background artwork.
    - Moved the GamerScore section below the central identity circle.
    - Placed Xbox ONLINE placeholder and live home-Wi-Fi icon together
      beneath the GamerScore section.
    - No change to boot animation, SD/CBP player, staging, NVS, setup portal,
      Wi-Fi connection, background asset, fonts, or background artwork.


  v1.21:
    - Based directly on the tested UI structure of v1.20.
    - Adds a very slow RGB-style breathing glow around the central
      gamer profile placeholder.
    - The animation is procedural: no animation assets, no SD reads,
      no heap allocation and no new framebuffer.
    - Uses fixed concentric rings plus a subtle moving highlight.
    - Updates at a deliberately modest rate so the ESP32 is not being
      asked to behave like a full-screen video renderer.
    - Background image, text positions, fonts, NVS, SD, boot player,
      configuration portal and Wi-Fi logic are otherwise unchanged.
    - Purpose: prove a living profile UI can run continuously without
      disturbing the existing product path.


  v1.22:
    - Based directly on v1.21.
    - Replaces the static PBG profile background + procedural ring glow
      with a continuously looping CBP1/LZ4 animated energy-field background.
    - Uses the proven CBP player architecture: 32 KiB staging, two 32 KiB
      DMA buffers, indexed palette frames and direct DMA to the TFT.
    - New profile asset: /profile_energy.CBP, 360x360, 48 frames, 8 FPS,
      6-second loop.
    - Live profile data remains a separate overlay drawn after every
      background frame.
    - Background file is opened once and kept open; frames are sought by
      indexed offset. No per-frame heap allocation and no per-frame file open.
    - No change to the proven Xbox boot animation/player, SD bus, TFT bus,
      NVS/configuration path or home-Wi-Fi path.
    - v1.21 procedural glow is removed from this active path because the
      animated asset provides the requested richer energy-field effect.


  v1.23:
    - Built from v1.22 without changing the animated-background architecture.
    - Corrected the supplied Lumina-derived profile animation asset by
      removing the visible source watermark before firmware encoding.
    - Fixed profile animation startup ordering so the animated asset is
      validated/opened before the first profile frame is rendered.
    - No change to the Xbox boot player, SD bus, staging, DMA buffers,
      product flow, NVS, Wi-Fi or live overlay architecture.


  v1.24:
    - Based directly on the tested v1.23 source.
    - ONE functional correction: the animated profile index validator
      no longer incorrectly rejects a valid frame because the TOTAL
      storedBytes across all of its blocks can exceed 40 KiB.
    - Library evidence shows CBP frame entries may contain multiple
      independent <=16,384-byte blocks; total frame storage is not
      required to fit inside the staging buffer.
    - The v1.23 asset contains 48 frames, 8 blocks per frame, and valid
      16 KiB-or-less block sizing, but its complete first-frame payload
      is ~127 KiB including palette/compressed blocks. That is valid for
      streaming through the existing 32 KiB staging buffer.
    - v1.24 validates each index entry against the actual file size
      instead, while retaining the existing per-block/staging limits.
    - No change to Xbox boot playback, SD configuration, NVS, setup,
      Wi-Fi, profile layout, fonts, or the animation asset.
    - Purpose: allow the supplied animated energy field to reach the
      already-written streaming/DMA playback code.


  v1.25:
    - Based directly on v1.24.
    - Profile energy source rebuilt from the original Lumina 24 FPS video.
    - Uses every second source frame: 85 frames @ 12 FPS.
    - Original source watermark region cleaned using the same established
      cleanup method used for the accepted 8 FPS asset.
    - Profile animation capacity raised from 64 to 96 indexed frames so
      the 85-frame asset fits safely.
    - Profile animation frame interval changed from 125 ms to 83 ms.
    - No change to Xbox boot player, SD hardware, NVS, setup portal,
      home Wi-Fi, profile overlay layout, staging size or DMA buffers.

  v1.26:
    - Based directly on the tested v1.25 source.
    - Fixes the visible profile overlay flashing at 12 FPS. v1.25
      repainted the entire 360x360 frame and then redrew all profile text
      on every animation frame.
    - After the initial full profile render, v1.26 updates only a central
      360px-wide x 128px-high energy band (Y=104..231).
    - XBOX, Gamer ID, GamerScore and the lower status row are outside the
      repeatedly rewritten band and therefore remain stable.
    - The central avatar placeholder is redrawn after each band update
      because it deliberately sits within the moving energy field.
    - Home-Wi-Fi status changes update only the lower status area rather
      than repainting the entire profile.
    - No change to the proven Xbox boot player, SD init, CBP codec, NVS,
      setup portal, Wi-Fi connection mechanism, profile layout or asset.
    - No new framebuffer or per-frame heap allocation introduced.


  v1.27:
    - Based directly on v1.26 and the tested v1.25 full-frame profile player.
    - Fixes the v1.26 animated-band decoder path.
    - The v1.26 path used a paired-block reader that advanced the CBP block
      loop manually; although logically intended to cover each block once,
      it departed from the already-proven playProfileFramePipelined() flow.
    - v1.27 replaces that section with the same single-prepare / optional
      next-prepare / swap sequence used by the proven player, while only
      transmitting the intersection of each decoded block with the central
      energy band.
    - Profile text and lower status remain outside the rewritten band.
    - The central avatar placeholder is redrawn after a successful band
      frame because it occupies the animated area.
    - No changes to Xbox boot playback, SD init, CBP codec, NVS, setup portal,
      Wi-Fi startup, profile layout, staging size, or DMA buffer sizes.
    - Purpose: restore actual moving energy while keeping the overlay stable.


  v1.28:
    - Based directly on the tested v1.27.
    - Keeps the proven 12 FPS animated profile player and stable live
      overlay behaviour.
    - Replaces the hard rectangular energy-band write boundary with a
      wide elliptical/rounded mask centred on the profile avatar.
    - Pixels outside the mask are left on the already-rendered first
      animation frame, removing the visible rectangular video window.
    - No new framebuffer, heap allocation, SD format, codec, player
      architecture, boot path, NVS, setup portal, or Wi-Fi mechanism.
    - Purpose: make the moving energy feel embedded in the artwork rather
      than appearing as a rectangular strip.


  v1.29:
    - Based directly on v1.28.
    - Fixes the v1.28 masked band compositor implementation only.
    - Replaces the complex pixel-walking / skip logic with one deterministic
      horizontal DMA span per display row inside the ellipse.
    - Each row is clipped against the current decoded CBP block, so source
      offsets remain exact even when a block starts/ends part-way through a row.
    - This removes the v1.28 source-position ambiguity that can leave black
      horizontal seams or fail to visibly advance the moving field.
    - The animation frame index still advances exactly once per successful
      frame, using the same 12 FPS timing.
    - No asset, codec, SD path, DMA buffer size, boot animation, NVS,
      setup portal, Wi-Fi or profile data layout changes.


  v1.30:
    - Based directly on tested v1.29 and its 12 FPS full-frame profile asset.
    - Replaces the runtime pixel/row masking experiment with a PC-built
      360x140 animated patch at Y=110.
    - The patch was generated from the original 640x640 / 24 FPS Lumina
      source, sampled every second frame, then precomposited against the
      exact decoded v1.29 frame-0 background.
    - Patch edges are feathered to the static base and the central 70px
      avatar zone is held exactly static.
    - Runtime writes the patch as one normal contiguous CBP/DMA rectangle,
      reusing the proven streaming/decode path instead of per-pixel masking.
    - The static profile text and avatar are not redrawn per animation frame.
    - Existing Xbox boot animation, SD path, staging, DMA buffers, NVS,
      configuration portal, Wi-Fi and profile data layout are preserved.
    - Purpose: eliminate the black-line artefacts while retaining genuine
      moving energy behind the profile picture.
    - Corrected one diagnostic message so it reports the actual 32 KiB staging
      allocation used by the v1.16+ product path.

  v1.31:
    - Based directly on the distributed v1.30 source.
    - Compile-only correction: remove the stale duplicate full-frame
      profile renderer that referenced undefined profileAnimFrame.
    - Compile-only correction: use the existing profilePatchNextMs
      scheduler in setup instead of undefined profileAnimNextMs.
    - No intended runtime behaviour change from v1.30.
    - Boot, animated patch compositor, SD/CBP/LZ4, NVS/configuration,
      Wi-Fi, profile layout, assets, pins, SPI frequencies, staging and
      DMA architecture are otherwise unchanged.


  v1.32:
    - Based directly on the tested v1.31 source.
    - Single controlled fix: explicitly seek the CBP file to the selected
      frame's indexed offset before decoding it.
    - v1.31 relied on the File object's previous sequential position.
      That works until the 85-frame patch loops back from the final frame
      to frame 0; at that point the file pointer is at EOF while the new
      frame expects data from the beginning, producing repeated
      "ERROR: stage read 0/32768".
    - Explicit indexed seeking also makes any future profile redraw that
      resets the frame index safe.
    - No change to asset, geometry, 12 FPS timing, codec, staging, DMA
      buffers, Xbox boot path, NVS, setup portal, Wi-Fi, or overlay.


  v1.33:
    - Based directly on the tested v1.32 source.
    - Single controlled change: eliminate simultaneous open SD File
      handles for the base profile frame and animated patch playback.
    - Evidence from v1.32: the CBP header/index validation succeeds for
      both assets, but the first patch frame still reports
      "ERROR: stage read 0/32768" even after an explicit indexed seek.
    - v1.32 therefore did not establish the old EOF hypothesis. The
      remaining concrete difference is that both profile CBP files are
      kept open at the same time on the same SD filesystem.
    - v1.33 closes the patch stream while the static base frame is rendered,
      closes the base stream immediately after that frame is written,
      then reopens the already-validated patch stream.
    - During later retention redraws, the same one-file-at-a-time rule is
      preserved.
    - No change to the CBP asset, decoder, staging size, DMA buffers,
      12 FPS timing, display geometry, boot animation, NVS, setup portal,
      Wi-Fi, or profile overlay.



  v1.39:
    - Based directly on v1.38 and the first physical V28 LCD test.
    - Removes the temporary TE avatar initials and cyan/blue circular placeholder.
    - Moves ONLINE and Wi-Fi status indicators to the left/right of the former
      avatar position, keeping both indicators white and within the central dark region.
    - Moves live Gamer ID text from Y=48 to Y=80 in the lower black/gloss region
      immediately below its previous position.
    - Fixes animation-loop scheduler recovery: when a frame overruns its deadline,
      the next due frame is allowed to render immediately instead of adding a full
      extra frame interval after the overrun. This specifically targets the visible
      pause at the 97->0 wrap while retaining all 97 frames at 12 FPS.
    - Scheduler timing upgraded from millisecond/83ms cadence to microsecond/~83.333ms
      cadence for more accurate 12 FPS pacing.
    - Product loop polling reduced from 30ms to 5ms so due animation frames are serviced
      promptly without changing the 12 FPS target.
    - No changes to the Lumina profile_energy.CBP asset or its colours.

  v1.38:
    - Based directly on corrected v1.37.
    - Replaces the two-layer profile energy system with ONE full-screen
      360x360 Lumina CBP1 animation at 12 FPS.
    - Uses all 97 Lumina source frames, producing an ~8.08 second loop.
    - Removes the runtime animated 360x140 patch layer entirely.
    - Background animation loops continuously to keep the small TFT visually active.
    - Live profile overlay remains a separate firmware layer.
    - Animation scheduler uses a deadline-based cadence to reduce accumulated timing drift.
    - New Lumina background uses its own source-derived 256-colour RGB565 palette;
      V27 colour assets are not used by this build.
    - No change to boot flow, NVS/configuration, Wi-Fi mechanism, TFT pins,
      SD pins, DMA buffer sizes or CBP1 decoder format.

  v1.36:
    - Based directly on the tested v1.35 source and corrected profile assets.
    - Single controlled compositor-order change: during full profile-screen
      reconstruction, render animated patch frame 0 before drawing the static
      profile foreground overlay.
    - This prevents the TE/avatar foreground from being briefly exposed and
      then covered by the opaque animated patch at startup and during retention
      redraws.
    - The patch remains the same 360x140 asset at Y=110 and the normal 12 FPS
      scheduler remains unchanged.
    - Corrected palette asset, CBP/LZ4 decoder, SD path, DMA, staging, boot,
      NVS, configuration, Wi-Fi, hardware pins and SPI frequencies are unchanged.

  v1.35:
    - Validation build based directly on v1.34.
    - No playback, decoder, DMA, SD, display, memory or asset changes.
    - Carries the corrected profile_energy_patch.CBP from v1.34 unchanged.
    - Purpose: physical validation of the confirmed RGB565 palette byte-order fix.

  v1.34:
    - Based directly on the tested v1.33 source.
    - Single controlled asset correction: fix RGB565 palette byte order
      in profile_energy_patch.CBP.
    - v1.33 streams the patch without stage-read failures, but the
      physical display shows severe false/psychedelic colours.
    - The actual patch-generation script packed palette RGB565 words in
      little-endian order, unlike the proven CBP asset path which stores
      display wire-order bytes.
    - Only the 256 palette words in the patch asset are byte-swapped;
      frame indices, compressed blocks, geometry, timing and payload sizes
      are unchanged. CRCs are updated.
    - Firmware playback logic is unchanged apart from version/changelog.
*/

/*
  UPCBadger v1.26
  96/192/256-COLOUR HC9 PLAYBACK
  256-COLOR PALETTE + LZ4 HC9 + DIRECT-DMA PIPELINE

  PURPOSE:
    Attack the remaining SD bottleneck without changing the
    physical 360x360 display or the current visual composition.

  CODEC:
    CBP1 = ConsoleBadger Palette Video, version 1
    256-color per-frame RGB565 palette
    8-bit palette indices
    independent 16 KiB LZ4 blocks
    palette + compressed indices are pre-built on the PC

  WHY:
    The current 330x350 RGB565 CBV carries 16 bits per pixel.
    This candidate carries 8 bits per pixel plus a 512-byte palette,
    then expands indices back to RGB565 immediately before DMA.

    The test asset was generated from the current visual master.
    Average RGB888 palette reconstruction error was measured at
    ~1.41 levels/channel on the cropped 330x350 image sequence.

  DISPLAY — LOCKED:
    MOSI 23
    SCLK 18
    CS   21
    DC   22
    RST   4
    TFT SPI = 80 MHz

  SD — LOCKED PINS:
    MOSI 27
    SCLK 25
    MISO 26
    CS   13

  IMPORTANT:
    Arduino ESP32 SD.h is retained.
    No custom File buffer is used.
    The previous 64 KiB File buffer test made performance much worse.

  MEMORY:
    32 KiB compressed staging
    2 x 32 KiB DMA buffers
    256-entry RGB565 palette = 512 bytes

  SAFETY:
    UNPLUG THE ESP32 BEFORE CHANGING WIRING OR SOLDERING.
    Keep this as the ONLY .ino file in its Arduino sketch folder.
*/


// ============================================================
// V1.48 NETWORK DIAGNOSTIC
// ------------------------------------------------------------
// Parent: V1.44 compile-repair firmware.
// Uses the GamerTag stored by the setup portal as the live
// OpenXBL lookup target. No real API key is stored in this source.
// Secrets live in local Secrets.h.
// ============================================================
#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <LovyanGFX.hpp>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Secrets.h"
#include <esp_system.h>
#include <esp_heap_caps.h>

// ============================================================
// V1.58
// ------------------------------------------------------------
// Fixes the V1.57 gamerpic compile failure by bridging Arduino
// ESP32 SD.h fs::File to LovyanGFX DataWrapper for streaming PNG
// decode. No image is loaded into a large RAM buffer.
// ============================================================

// ============================================================
// PINS
// ============================================================

#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   21
#define TFT_DC   22
#define TFT_RST   4

#define SD_MOSI 27
#define SD_SCLK 25
#define SD_MISO 26
#define SD_CS   13

// ============================================================
// MASTER DISPLAY / ACTIVE WINDOW
// ============================================================

// V1.41 uses ONE 360x360 master animation for both boot and Gamer ID.
#define PANEL_WIDTH  360
#define PANEL_HEIGHT 360

#define WIDTH  360
#define HEIGHT 360

#define WINDOW_X 0
#define WINDOW_Y 0

#define FRAME_PIXELS (WIDTH * HEIGHT)
#define DISPLAY_FRAME_BYTES (FRAME_PIXELS * 2)

// ============================================================
// SPI
// ============================================================

#define TFT_SPI_HZ 80000000UL
#define SD_SPI_HZ  40000000UL

// ============================================================
// CBP1
// ============================================================

#define CBP_FILE "/ConsoleBadger_MASTER.CBP"

#define CBP_VERSION 1
#define CBP_CODEC_PALETTE_LZ4 2

#define FRAME_COUNT_EXPECTED 181
#define MASTER_BOOT_END_FRAME 104UL
#define MASTER_LOOP_START_FRAME 104UL
#define MASTER_LOOP_END_FRAME 181UL
#define BLOCK_INDEX_BYTES 16384

#define DIRECT_DMA_FLAG 0x0002
#define PALETTE_FLAG     0x0004

#define CBP_HEADER_BYTES 64
#define CBP_INDEX_ENTRY_BYTES 16
#define CBP_DATA_OFFSET \
  (CBP_HEADER_BYTES + FRAME_COUNT_EXPECTED * CBP_INDEX_ENTRY_BYTES)

// Palette = 256 RGB565 entries = 512 bytes.
#define PALETTE_ENTRIES 256
#define PALETTE_BYTES   (PALETTE_ENTRIES * 2)

// ============================================================
// MEMORY
// ============================================================

// Two 32 KiB decoded DMA buffers.
// A compressed 16 KiB index block expands to exactly 32 KiB RGB565.
static uint8_t dmaBufferA[32768];
static uint8_t dmaBufferB[32768];

// Compressed CBP data staging.
// Normal 8-bit-capable RAM, not a DMA buffer.
//
// v1.16: reduced from 96 KiB to 32 KiB. The decoder only ever asks
// stageEnsure() for two things: the 512-byte palette, or one CBP
// block (8-byte CBPBlockHeader + up to BLOCK_INDEX_BYTES compressed
// payload = 16392 bytes max, verified against the struct/format
// constants below). 32 KiB gives ~2x headroom over that 16392-byte
// floor for read-ahead efficiency while staying far below the
// largest contiguous free block observed after SD init in the
// Library evidence. Every place that assumed the old 96 KiB capacity
// now references STAGE_BUFFER_BYTES instead, so this is the ONLY
// place the capacity is set.
#define STAGE_BUFFER_BYTES (32 * 1024)
static uint8_t *stageBuffer = nullptr;


// ============================================================
// PROFILE BACKGROUND (PBG1)
// ============================================================
//
// The profile background is a 360x360, 256-colour indexed image
// compressed as independent LZ4 blocks. It is rendered directly from
// the SD card using the EXISTING 32 KiB staging buffer and one of the
// existing 32 KiB DMA buffers. No 360x360 framebuffer is allocated.
//
#define PBG_VERSION 1
#define PBG_WIDTH  360
#define PBG_HEIGHT 360
#define PBG_PALETTE_ENTRIES 256
#define PBG_PALETTE_BYTES (PBG_PALETTE_ENTRIES * 2)
#define PBG_BLOCK_HEIGHT 40
#define PBG_BLOCK_COUNT 9
#define PBG_HEADER_BYTES 36
#define PBG_INDEX_ENTRY_BYTES 16

#pragma pack(push, 1)
struct PBGHeader
{
  char magic[4];
  uint16_t version;
  uint16_t width;
  uint16_t height;
  uint16_t paletteEntries;
  uint16_t blockHeight;
  uint16_t blockCount;
  uint16_t indexEntryBytes;
  uint16_t flags;
  uint32_t indexOffset;
  uint32_t dataOffset;
  uint32_t reserved0;
  uint32_t reserved1;
};

struct PBGIndexEntry
{
  uint32_t offset;
  uint32_t storedBytes;
  uint16_t rawBytes;
  uint16_t y;
  uint16_t height;
  uint16_t flags;
};
#pragma pack(pop)

static_assert(sizeof(PBGHeader) == PBG_HEADER_BYTES,
              "PBG header size mismatch");
static_assert(sizeof(PBGIndexEntry) == PBG_INDEX_ENTRY_BYTES,
              "PBG index entry size mismatch");
static_assert(PBG_BLOCK_HEIGHT * PBG_WIDTH <= BLOCK_INDEX_BYTES,
              "PBG block exceeds staging decoder limit");

// Current per-frame palette.
// File stores byte-swapped RGB565 so a direct uint16 load produces
// the correct MSB-first bytes in little-endian ESP32 memory.
static uint16_t palette565[PALETTE_ENTRIES];

// ============================================================
// LOVYANGFX
// ============================================================

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_GC9B72 panel;
  lgfx::Bus_SPI bus;

public:
  LGFX()
  {
    auto cfg = bus.config();

    cfg.spi_host  = VSPI_HOST;
    cfg.spi_mode  = 0;
    cfg.freq_write = TFT_SPI_HZ;
    cfg.freq_read  = 16000000;

    cfg.pin_sclk = TFT_SCLK;
    cfg.pin_mosi = TFT_MOSI;
    cfg.pin_miso = -1;
    cfg.pin_dc   = TFT_DC;

    cfg.dma_channel = SPI_DMA_CH_AUTO;

    bus.config(cfg);
    panel.setBus(&bus);

    auto pc = panel.config();

    pc.pin_cs   = TFT_CS;
    pc.pin_rst  = TFT_RST;
    pc.pin_busy = -1;

    pc.panel_width  = PANEL_WIDTH;
    pc.panel_height = PANEL_HEIGHT;
    pc.memory_width = PANEL_WIDTH;
    pc.memory_height = PANEL_HEIGHT;

    pc.offset_x = 0;
    pc.offset_y = 0;
    pc.offset_rotation = 0;

    pc.readable = false;
    pc.invert = false;
    pc.rgb_order = false;
{
  tft.setTextDatum(datum);
  tft.setFont(font);
  tft.setTextColor(color, TFT_BLACK);
  tft.drawString(value, x, y);
}

// ------------------------------------------------------------
// Gamer page
// ------------------------------------------------------------


// ------------------------------------------------------------
// ------------------------------------------------------------
// Animated profile energy-field background
// ------------------------------------------------------------

// Single full-screen Lumina animation.
#define PROFILE_ANIM_FILE CBP_FILE
#define PROFILE_ANIM_VERSION 1
#define PROFILE_ANIM_CODEC 2
#define PROFILE_ANIM_WIDTH 360
#define PROFILE_ANIM_HEIGHT 360
#define PROFILE_ANIM_MAX_FRAMES 200
#define PROFILE_ANIM_HEADER_BYTES 64
#define PROFILE_ANIM_INDEX_ENTRY_BYTES 16
#define PROFILE_ANIM_FPS_NUM 12
#define PROFILE_ANIM_FPS_DEN 1
#define PROFILE_ANIM_FRAME_INTERVAL_US 83333UL

static CBPHeader profileAnimHeader;
static CBPIndexEntry profileAnimIndex[PROFILE_ANIM_MAX_FRAMES];
static File profileAnimFile;
static bool profileAnimReady = false;
static uint32_t profileAnimFrame = 0;
static uint32_t profileAnimNextUs = 0;

static bool validateCBPIndex(
  File &file,
  CBPHeader &header,
  CBPIndexEntry *index,
  uint32_t maxFrames,
  uint16_t expectedWidth,
  uint16_t expectedHeight,
  const char *label
)
{
  if (
    file.read(
      (uint8_t *)&header,
      sizeof(header)
    ) != sizeof(header)
  )
  {
    Serial.print("[PROFILE] ");
    Serial.print(label);
    Serial.println(" header read failed.");
    return false;
  }

  if (
    memcmp(header.magic, "CBP1", 4) != 0 ||
    header.version != PROFILE_ANIM_VERSION ||
    header.codec != PROFILE_ANIM_CODEC ||
    header.width != expectedWidth ||
    header.height != expectedHeight ||
    header.frameCount == 0 ||
    header.frameCount > maxFrames ||
    header.displayFrameBytes !=
      (uint32_t)expectedWidth * expectedHeight * 2UL ||
    header.codedFrameBytes !=
      (uint32_t)expectedWidth * expectedHeight ||
    header.blockIndexBytes != BLOCK_INDEX_BYTES ||
    header.indexEntryBytes != sizeof(CBPIndexEntry) ||
    !(header.flags & DIRECT_DMA_FLAG) ||
    !(header.flags & PALETTE_FLAG) ||
    header.indexOffset != PROFILE_ANIM_HEADER_BYTES ||
    header.dataOffset !=
      PROFILE_ANIM_HEADER_BYTES +
      header.frameCount * PROFILE_ANIM_INDEX_ENTRY_BYTES
  )
  {
    Serial.print("[PROFILE] ");
    Serial.print(label);
    Serial.println(" header invalid.");
    return false;
  }

  file.seek(header.indexOffset);

  const size_t bytes =
    header.frameCount * sizeof(CBPIndexEntry);

  if (
    file.read(
      (uint8_t *)index,
      bytes
    ) != bytes
  )
  {
    Serial.print("[PROFILE] ");
    Serial.print(label);
    Serial.println(" index read failed.");
    return false;
  }

  const uint64_t fileSize =
    (uint64_t)file.size();

  for (uint32_t i = 0; i < header.frameCount; ++i)
  {
    const CBPIndexEntry &e = index[i];
    const uint64_t end =
      (uint64_t)e.offset + (uint64_t)e.storedBytes;

    if (
      e.offset < header.dataOffset ||
      e.storedBytes == 0 ||
      e.blockCount == 0 ||
      end > fileSize
    )
    {
      Serial.print("[PROFILE] ");
      Serial.print(label);
      Serial.println(" index invalid.");
      return false;
    }
  }

  return true;
}

static bool loadProfileAnimIndex()
{
  profileAnimFile =
    SD.open(
      PROFILE_ANIM_FILE,
      FILE_READ
    );

  if (!profileAnimFile)
  {
    Serial.println(
      "[PROFILE] Base animation asset not found."
    );
    return false;
  }

  if (!validateCBPIndex(
        profileAnimFile,
        profileAnimHeader,
        profileAnimIndex,
        PROFILE_ANIM_MAX_FRAMES,
        PROFILE_ANIM_WIDTH,
        PROFILE_ANIM_HEIGHT,
        "Base animation"
      ))
  {
    profileAnimFile.close();
    return false;
  }

  profileAnimReady = true;

  Serial.print(
    "[PROFILE] Master animation ready: "
  );
  Serial.print(profileAnimHeader.frameCount);
  Serial.println(" frames @ 12 FPS.");

  return true;
}

static uint32_t profileProtectMask = PROFILE_PROTECT_PIC;

// Forward declaration: the compositor helper is defined with the other
// profile UI helpers later in the source, but the frame writer uses it.
static inline void applyProfileStaticOverlayToRow(
  uint16_t *rowBuffer,
  int rowY,
  int xStart,
  int count
);

static bool playProfileFramePipelined(
  File &file,
  const CBPIndexEntry *indexTableLocal,
  uint32_t frameIndex,
  uint32_t frameWidth,
  uint32_t frameHeight,
  uint32_t displayY,
  uint32_t &frameUs,
  uint32_t &sdUs,
  uint32_t &decodeUs,
  uint32_t &dmaUs,
  uint32_t &stageUs
)
{
  const CBPIndexEntry &entry = indexTableLocal[frameIndex];

  if (!file.seek(entry.offset))
  {
    Serial.println("[PROFILE] CBP frame seek failed.");
    return false;
  }

  StageReader reader;
  reader.file = &file;
  reader.beginPos = 0;
  reader.endPos = 0;
  reader.fileRemaining = entry.storedBytes;

  uint8_t *current = dmaBufferA;
  uint8_t *next = dmaBufferB;
  size_t currentPixels = 0;
  size_t nextPixels = 0;
  // The overlay row buffer is needed only for this frame write. Keep it on
  // the task stack instead of DRAM .bss so the existing memory budget is not
  // increased. The physical animation width is fixed at 360 pixels.
  uint16_t profileOverlayRow[360];
  uint32_t frameStart = micros();

  if (!loadPalette(reader))
    return false;

  if (!prepareNextBlock(reader, current, currentPixels, decodeUs))
    return false;

  tft.startWrite();

  uint32_t pixelsSent = 0;
  const uint16_t blockCount = entry.blockCount;

  const uint32_t totalPixels = frameWidth * frameHeight;

  for (uint16_t block = 0; block < blockCount; ++block)
  {
    const uint32_t blockStart = pixelsSent;
    const uint32_t blockEnd = blockStart + currentPixels;
    uint32_t cursor = blockStart;

    // V2.25: protect only the actual circular gamerpic image.
    // Text/status/score are transparent overlays and are redrawn after each frame.
    while (cursor < blockEnd)
    {
      const uint32_t row = cursor / frameWidth;
      const uint32_t col = cursor % frameWidth;
      const uint32_t rowEnd = min(blockEnd, (row + 1UL) * frameWidth);
      uint32_t nextCut = rowEnd;

      bool insideProtected = false;
      uint32_t protectedEnd = rowEnd;

      // V2.25: static profile text/status is composited into the outgoing
      // animation row. This preserves the live pixels without a separate
      // drawString/drawLine pass after every frame.
      const bool rowHasStaticOverlay =
        profileOverlayRasterReady &&
        (
          (row >= (uint32_t)profileGamerMaskY0 &&
           row < (uint32_t)(profileGamerMaskY0 + PROFILE_GAMER_MASK_H)) ||
          (row >= (uint32_t)PROFILE_STATUS_Y &&
           row < (uint32_t)(PROFILE_STATUS_Y + PROFILE_STATUS_THICKNESS)) ||
          (row >= (uint32_t)profileScoreMaskY0 &&
           row < (uint32_t)(profileScoreMaskY0 + PROFILE_SCORE_MASK_H))
        );

      if (rowHasStaticOverlay)
      {
        const int rowCount =
          (int)(rowEnd - cursor);

        memcpy(
          profileOverlayRow,
          reinterpret_cast<uint16_t *>(current) + (cursor - blockStart),
          (size_t)rowCount * sizeof(uint16_t)
        );

        applyProfileStaticOverlayToRow(
          profileOverlayRow,
          (int)row,
          (int)col,
          rowCount
        );

        tft.setAddrWindow(
          (int)col,
          displayY + (int)row,
          rowCount,
          1
        );

        const uint32_t waitStart = micros();
        tft.writePixelsDMA(
          profileOverlayRow,
          rowCount,
          false
        );
        tft.waitDMA();
        dmaUs += micros() - waitStart;

        cursor = rowEnd;
        continue;
      }

      if (profileProtectMask != 0)
      {
        // V2.25: preserve ONLY the actual circular gamerpic image.
        if (
          (profileProtectMask & PROFILE_PROTECT_PIC) &&
          row >= (uint32_t)(PROFILE_PIC_CENTER_Y - PROFILE_PIC_IMAGE_RADIUS) &&
          row <= (uint32_t)(PROFILE_PIC_CENTER_Y + PROFILE_PIC_IMAGE_RADIUS)
        )
        {
          const int dy =
            (int)row -
            PROFILE_PIC_CENTER_Y;

          const int radius =
            PROFILE_PIC_IMAGE_RADIUS;

          const int remaining =
            radius * radius -
            dy * dy;

          if (remaining >= 0)
          {
            const int half =
              (int)sqrtf(
                (float)remaining
              );

            const uint32_t picX0 =
              (uint32_t)(
                PROFILE_PIC_CENTER_X -
                half
              );

            const uint32_t picX1 =
              (uint32_t)(
                PROFILE_PIC_CENTER_X +
                half +
                1
              );

            if (
              col >= picX0 &&
              col < picX1
            )
            {
              insideProtected =
                true;

              protectedEnd =
                min(
                  protectedEnd,
                  picX1
                );
            }
            else if (col < picX0)
            {
              nextCut =
                min(
                  nextCut,
                  picX0 +
                  row * frameWidth
                );
            }
          }
        }
      }

      if (insideProtected)
      {
        cursor = min(blockEnd, protectedEnd + row * frameWidth);
        continue;
      }

      if (nextCut <= cursor)
        break;

      const uint32_t count = nextCut - cursor;
      const uint32_t writeRow = cursor / frameWidth;
      const uint32_t writeCol = cursor % frameWidth;

      tft.setAddrWindow(
        writeCol,
        displayY + writeRow,
        count <= (frameWidth - writeCol) ? count : (frameWidth - writeCol),
        1
      );

      uint32_t sent = 0;
      while (sent < count)
      {
        const uint32_t rowRemaining = frameWidth - ((writeCol + sent) % frameWidth);
        const uint32_t chunk = min(count - sent, rowRemaining);

        if (sent != 0)
        {
          tft.setAddrWindow(
            (writeCol + sent) % frameWidth,
            displayY + (cursor + sent) / frameWidth,
            chunk,
            1
          );
        }

        const uint32_t waitStart = micros();
        tft.writePixelsDMA(
          reinterpret_cast<uint16_t *>(current) + (cursor - blockStart) + sent,
          chunk,
          false
        );
        tft.waitDMA();
        dmaUs += micros() - waitStart;
        sent += chunk;
      }

      cursor = nextCut;
    }

    bool haveNext = false;
    if (block + 1 < blockCount)
    {
      haveNext = prepareNextBlock(reader, next, nextPixels, decodeUs);
      if (!haveNext)
      {
        tft.waitDMA();
        tft.endWrite();
        return false;
      }
    }

    pixelsSent += currentPixels;

    if (!haveNext)
      break;

    uint8_t *tmp = current;
    current = next;
    next = tmp;
    currentPixels = nextPixels;
    nextPixels = 0;
  }

  tft.endWrite();

  frameUs = micros() - frameStart;

  if (reader.fileRemaining != 0 ||
      reader.beginPos != reader.endPos ||
      pixelsSent != totalPixels)
  {
    Serial.println("[PROFILE] Profile animation frame validation failed.");
    return false;
  }

  sdUs = (uint32_t)reader.sdUs;
  stageUs = (uint32_t)reader.stageUs;
  return true;
}

static bool renderProfileAnimationFrame(uint32_t frameIndex, uint32_t protectMask = PROFILE_PROTECT_PIC)
{
  profileProtectMask = protectMask;
  if (!profileAnimReady || !profileAnimFile)
    return false;

  if (frameIndex >= profileAnimHeader.frameCount)
    frameIndex = 0;

  uint32_t frameUs = 0;
  uint32_t sdUs = 0;
  uint32_t decodeUs = 0;
  uint32_t dmaUs = 0;
  uint32_t stageUs = 0;

  return playProfileFramePipelined(
    profileAnimFile,
    profileAnimIndex,
    frameIndex,
    PROFILE_ANIM_WIDTH,
    PROFILE_ANIM_HEIGHT,
    0,
    frameUs,
    sdUs,
    decodeUs,
    dmaUs,
    stageUs
  );
}

// ------------------------------------------------------------
// Profile text / icon helpers
// ------------------------------------------------------------

static uint16_t profileWhite(uint8_t amount = 255)
{
  return productScale565(
    productRGB565(235, 237, 224),
    amount
  );
}

static void profileText(
  const String &value,
  int x,
  int y,
  uint16_t color,
  const lgfx::IFont *font,
  textdatum_t datum = textdatum_t::middle_center
)
{
  tft.setTextDatum(datum);
  tft.setFont(font);
  tft.setTextColor(color);
  tft.drawString(value, x, y);
}


static void profileOrbitronText(
  const String &value,
  int x,
  int y,
  uint16_t color,
  float scale,
  textdatum_t datum = textdatum_t::middle_center
)
{
  tft.setTextDatum(datum);
  tft.setFont(
    &fonts::Orbitron_Light_24
  );
  tft.setTextSize(
    scale
  );
  tft.setTextColor(
    color
  );

  tft.drawString(
    value,
    x,
    y
  );

  // Always restore the global text scale after the scaled Orbitron draw.
  tft.setTextSize(
    1.0f
  );
}

static void profileMaskClear(
  uint8_t *mask,
  size_t bytes
)
{
  memset(mask, 0, bytes);
}

static inline void profileMaskSet(
  uint8_t *mask,
  size_t index
)
{
  mask[index >> 3] |=
    (uint8_t)(1u << (index & 7));
}

static inline bool profileMaskGet(
  const uint8_t *mask,
  size_t index
)
{
  return (
    mask[index >> 3] &
    (uint8_t)(1u << (index & 7))
  ) != 0;
}

static bool buildProfileTextMask(
  const String &value,
  float scale,
  int width,
  int height,
  uint8_t *mask
)
{
  profileMaskClear(
    mask,
    (size_t)((width * height + 7) / 8)
  );

  LGFX_Sprite sprite(
    &tft
  );

  sprite.setColorDepth(8);

  if (!sprite.createSprite(width, height))
  {
    Serial.println(
      "[PROFILE] Static text mask sprite allocation failed."
    );
    return false;
  }

  sprite.fillScreen(0);
  sprite.setTextDatum(
    textdatum_t::middle_center
  );
  sprite.setFont(
    &fonts::Orbitron_Light_24
  );
  sprite.setTextSize(
    scale
  );
  sprite.setTextColor(
    255
  );

  sprite.drawString(
    value,
    width / 2,
    height / 2
  );

  uint8_t *buffer =
    static_cast<uint8_t *>(
      sprite.getBuffer()
    );

  if (!buffer)
  {
    sprite.deleteSprite();
    Serial.println(
      "[PROFILE] Static text mask buffer unavailable."
    );
    return false;
  }

  const size_t pixels =
    (size_t)width * height;

  for (size_t i = 0; i < pixels; ++i)
  {
    if (buffer[i] != 0)
      profileMaskSet(mask, i);
  }

  sprite.deleteSprite();
  return true;
}

static bool prepareProfileOverlayRaster()
{
  const String gamerId =
    liveGamerID.length()
      ? liveGamerID
      : (
          savedGamerID.length()
            ? savedGamerID
            : "PLAYER"
        );

  String displayGamerId =
    gamerId;

  const float orbitron16 =
    16.0f / 24.0f;

  tft.setFont(
    &fonts::Orbitron_Light_24
  );
  tft.setTextSize(
    orbitron16
  );

  if (tft.textWidth(displayGamerId) > 175)
  {
    displayGamerId =
      displayGamerId.substring(0, 15);
  }

  profileGamerWidth =
    tft.textWidth(displayGamerId);

  tft.setTextSize(1.0f);

  const bool gamerOK =
    buildProfileTextMask(
      displayGamerId,
      orbitron16,
      PROFILE_GAMER_MASK_W,
      PROFILE_GAMER_MASK_H,
      profileGamerMask
    );

  const String score =
    String(liveGamerScore);

  const bool scoreOK =
    buildProfileTextMask(
      score,
      24.0f / 24.0f,
      PROFILE_SCORE_MASK_W,
      PROFILE_SCORE_MASK_H,
      profileScoreMask
    );

  if (!gamerOK || !scoreOK)
  {
    profileOverlayRasterReady = false;
    return false;
  }

  profileGamerMaskX0 =
    PROFILE_PIC_CENTER_X - (PROFILE_GAMER_MASK_W / 2);
  profileGamerMaskY0 =
    97 - (PROFILE_GAMER_MASK_H / 2);
  profileScoreMaskX0 =
    PROFILE_PIC_CENTER_X - (PROFILE_SCORE_MASK_W / 2);
  profileScoreMaskY0 =
    255 - (PROFILE_SCORE_MASK_H / 2);

  profileOverlayRasterReady = true;
  return true;
}

static inline void applyProfileStaticOverlayToRow(
  uint16_t *rowBuffer,
  int rowY,
  int xStart,
  int count
)
{
  if (!profileOverlayRasterReady || count <= 0)
    return;

  const uint16_t white =
    profileWhite(255);

  // GamerTag glyph pixels.
  if (
    rowY >= profileGamerMaskY0 &&
    rowY < profileGamerMaskY0 + PROFILE_GAMER_MASK_H
  )
  {
    const int localY =
      rowY - profileGamerMaskY0;

    for (int i = 0; i < count; ++i)
    {
      const int x = xStart + i;
      if (
        x >= profileGamerMaskX0 &&
        x < profileGamerMaskX0 + PROFILE_GAMER_MASK_W
      )
      {
        const int localX =
          x - profileGamerMaskX0;
        const size_t idx =
          (size_t)localY * PROFILE_GAMER_MASK_W + localX;

        if (profileMaskGet(profileGamerMask, idx))
          rowBuffer[i] = white;
      }
    }
  }

  // Split live status bar. The bar is represented directly because it is
  // a solid 2px geometry rather than a font mask.
  if (
    rowY >= PROFILE_STATUS_Y &&
    rowY < PROFILE_STATUS_Y + PROFILE_STATUS_THICKNESS
  )
  {
    const int statusX0 =
      PROFILE_PIC_CENTER_X - profileGamerWidth / 2;

    const int leftWidth =
      (profileGamerWidth - PROFILE_STATUS_GAP) / 2;

    const int rightWidth =
      profileGamerWidth - PROFILE_STATUS_GAP - leftWidth;

    const int rightX =
      statusX0 + leftWidth + PROFILE_STATUS_GAP;

    const uint16_t lime =
      productRGB565(154, 245, 42);

    const uint16_t xblColor =
      (profileBarXblKnown && profileBarXblOnline)
        ? lime
        : white;

    const uint16_t wifiColor =
      profileBarWifiConnected
        ? lime
        : white;

    for (int i = 0; i < count; ++i)
    {
      const int x =
        xStart + i;

      if (
        x >= statusX0 &&
        x < statusX0 + leftWidth
      )
      {
        rowBuffer[i] = xblColor;
      }
      else if (
        x >= rightX &&
        x < rightX + rightWidth
      )
      {
        rowBuffer[i] = wifiColor;
      }
    }
  }

  // GamerScore glyph pixels.
  if (
    rowY >= profileScoreMaskY0 &&
    rowY < profileScoreMaskY0 + PROFILE_SCORE_MASK_H &&
    liveProfileInitialised
  )
  {
    const int localY =
      rowY - profileScoreMaskY0;

    for (int i = 0; i < count; ++i)
    {
      const int x = xStart + i;
      if (
        x >= profileScoreMaskX0 &&
        x < profileScoreMaskX0 + PROFILE_SCORE_MASK_W
      )
      {
        const int localX =
          x - profileScoreMaskX0;
        const size_t idx =
          (size_t)localY * PROFILE_SCORE_MASK_W + localX;

        if (profileMaskGet(profileScoreMask, idx))
          rowBuffer[i] = white;
      }
    }
  }
}

static void serviceProfileStatusBars()
{
  if (!productProfileActive)
    return;

  const uint32_t now = millis();

  if (
    now - lastProfileStatusRefreshMs <
    PROFILE_STATUS_REFRESH_INTERVAL_MS
  )
  {
    return;
  }

  lastProfileStatusRefreshMs = now;

  profileBarWifiConnected =
    WiFi.status() == WL_CONNECTED;

  profileBarXblKnown =
    liveXboxPresenceKnown;

  profileBarXblOnline =
    liveXboxPresenceKnown && liveXboxOnline;
}

static void drawProfileWifiIcon(
  bool connected,
  uint8_t brightness,
  int cx,
  int cy
)
{
  const uint16_t c =
    connected
      ? profileWhite(brightness)
      : productScale565(
          productRGB565(90, 92, 86),
          brightness
        );

  for (int band = 0; band < 3; ++band)
  {
    const float radius =
      5.0f + band * 3.0f;

    const float start =
      -2.45f;

    const float end =
      -0.70f;

    const int segments =
      8;

    int px =
      cx + (int)(cosf(start) * radius);

    int py =
      cy + (int)(sinf(start) * radius);

    for (int i = 1; i <= segments; ++i)
    {
      const float a =
        start +
        (end - start) *
        ((float)i / segments);

      const int nx =
        cx + (int)(cosf(a) * radius);

      const int ny =
        cy + (int)(sinf(a) * radius);

      tft.drawLine(
        px,
        py,
        nx,
        ny,
        c
      );

      px = nx;
      py = ny;
    }
  }

  tft.fillCircle(
    cx,
    cy + 5,
    2,
    c
  );
}

static const char *PROFILE_GAMERPIC_FILE = "/upcbadger_gamerpic.png";

static bool downloadLiveGamerPicToSD()
{
  badgerDiagCheckpoint(
    DIAG_GAMERPIC_DOWNLOAD_BEGIN,
    "GAMERPIC_DOWNLOAD_BEGIN"
  );

  if (liveGamerPicURL.length() == 0)
    return false;

  WiFiClientSecure picClient;
  picClient.setInsecure();
  HTTPClient picHttp;
  picClient.setHandshakeTimeout(8);
  picHttp.setConnectTimeout(5000);
  picHttp.setTimeout(8000);
  picHttp.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);

  Serial.println("[API] Downloading gamerpic to SD cache...");

  Serial.print("[API] Gamerpic URL: ");
  Serial.println(liveGamerPicURL);

  if (!picHttp.begin(picClient, liveGamerPicURL))
  {
    Serial.println("[API] Gamerpic HTTP begin failed.");
    return false;
  }

  picHttp.addHeader("Accept", "image/png,image/*");
  const int code = picHttp.GET();
  Serial.print("[API] Gamerpic HTTP status: ");
  Serial.println(code);

  if (code != HTTP_CODE_OK)
  {
    picHttp.end();
    return false;
  }

  const int total = picHttp.getSize();
  if (total > 131072)
  {
    Serial.println("[API] Gamerpic rejected: larger than 128 KiB.");
    picHttp.end();
    return false;
  }

   const name=document.createElement('b');
   name.textContent=n.ssid;

   const meta=document.createElement('div');
   meta.className='meta';
   meta.textContent=n.rssi+' dBm · '+(n.secure?'Secured':'Open');

   row.appendChild(name);
   row.appendChild(meta);

   row.onclick=()=>{
    document.getElementById('ssid').value=n.ssid;
    status.textContent='Selected '+n.ssid;
    document.getElementById('ssid').scrollIntoView({
      behavior:'smooth',
      block:'center'
    });
   };

   list.appendChild(row);
  });
 }
 catch(e){
  status.textContent='Scan failed. Enter the Wi-Fi name manually.';
 }
}
</script>

</body>
</html>
)HTML";

// ------------------------------------------------------------
// Browser handlers
// ------------------------------------------------------------

static String jsonEscape(
  const String &value
)
{
  String out;
  out.reserve(value.length() + 8);

  for (
    size_t i = 0;
    i < value.length();
    ++i
  )
  {
    char c = value[i];

    if (c == '\\' || c == '"')
    {
      out += '\\';
      out += c;
    }
    else if (c == '\n' || c == '\r')
    {
      out += ' ';
    }
    else
    {
      out += c;
    }
  }

  return out;
}

static void handleSetupRoot()
{
  Serial.println(
    "[AP] HTTP GET /"
  );

  setupServer.send(
    200,
    "text/html",
    SETUP_HTML
  );
}

static void handleSetupScan()
{
  Serial.println(
    "[SCAN] Starting Wi-Fi scan..."
  );

  // Keep the setup AP alive while scanning.
  WiFi.mode(WIFI_AP_STA);

  int16_t count =
    WiFi.scanNetworks(
      false,
      false
    );

  Serial.print(
    "[SCAN] Networks found: "
  );

  Serial.println(
    count
  );

  if (count < 0)
  {
    WiFi.scanDelete();

    setupServer.send(
      500,
      "application/json",
      "[]"
    );

    Serial.println(
      "[SCAN] Scan failed."
    );

    return;
  }

  String json = "[";
  bool first = true;

  for (int i = 0; i < count; ++i)
  {
    String ssid = WiFi.SSID(i);

    if (ssid.length() == 0)
      continue;

    if (!first)
      json += ",";

    first = false;

    bool secure =
      WiFi.encryptionType(i) != WIFI_AUTH_OPEN;

    json += "{\"ssid\":\"";
    json += jsonEscape(ssid);
    json += "\",\"rssi\":";
    json += String(WiFi.RSSI(i));
    json += ",\"secure\":";
    json += secure
      ? "true"
      : "false";
    json += "}";
  }

  json += "]";

  setupServer.send(
    200,
    "application/json",
    json
  );

  WiFi.scanDelete();

  Serial.println(
    "[SCAN] Results sent to browser."
  );
}

static void handleSetupSave()
{
  Serial.println(
    "[AP] HTTP POST /save"
  );

  String ssid =
    setupServer.arg("ssid");

  String pass =
    setupServer.arg("password");

  String gt =
    setupServer.arg("gamertag");

  String mode =
    setupServer.arg("mode");

  ssid.trim();
  gt.trim();

  int selectedMode =
    mode.toInt();

  // Only Xbox has a real animation asset in this build.
  selectedMode = MODE_XBOX;

  if (
    ssid.isEmpty() ||
    gt.isEmpty()
  )
  {
    setupServer.send(
      400,
      "text/plain",
      "Wi-Fi network and Gamer ID are required."
    );

    return;
  }

  Serial.print(
    "[SETUP] Saving Wi-Fi SSID: "
  );

  Serial.println(
    ssid
  );

  Serial.print(
    "[SETUP] Saving Gamer ID: "
  );

  Serial.println(
    gt
  );

  setupPrefs.begin(
    "upcbadger",
    false
  );

  setupPrefs.putString(
    "ssid",
    ssid
  );

  setupPrefs.putString(
    "pass",
    pass
  );

  setupPrefs.putString(
    "gt",
    gt
  );

  setupPrefs.putUChar(
    "mode",
    (uint8_t)selectedMode
  );

  setupPrefs.end();

  Serial.println(
    "[SETUP] NVS save complete."
  );

  setupServer.send(
    200,
    "text/html",
    "<html><body style='font-family:Arial;background:#000;color:#fff;text-align:center;padding:40px'><h1>Saved.</h1><p>Connecting ConsoleBadger to your Wi-Fi...</p></body></html>"
  );

  delay(900);
  ESP.restart();
}

// ------------------------------------------------------------
// Load / connect
// ------------------------------------------------------------

static bool loadProductSettings()
{
  setupPrefs.begin(
    "upcbadger",
    true
  );

  savedSSID =
    setupPrefs.getString(
      "ssid",
      ""
    );

  savedPassword =
    setupPrefs.getString(
      "pass",
      ""
    );

  savedGamerID =
    setupPrefs.getString(
      "gt",
      ""
    );

  uint8_t savedMode =
    setupPrefs.getUChar(
      "mode",
      MODE_XBOX
    );

  setupPrefs.end();

  if (savedMode > MODE_NINTENDO)
    savedMode = MODE_XBOX;

  consoleMode =
    (ConsoleMode)savedMode;

  // PS/Nintendo assets are not ready yet.
  if (consoleMode != MODE_XBOX)
  {
    Serial.println(
      "[SETUP] Non-Xbox mode requested; using Xbox asset for V1."
    );

    consoleMode = MODE_XBOX;
  }

  return (
    savedSSID.length() > 0 &&
    savedGamerID.length() > 0
  );
}

static bool connectHomeWiFi()
{
  Serial.println();
  Serial.println(
    "[WIFI] Connecting to saved network..."
  );

  WiFi.mode(WIFI_STA);

  WiFi.setHostname(
    "consolebadger"
  );

  WiFi.begin(
    savedSSID.c_str(),
    savedPassword.c_str()
  );

  tft.fillScreen(
    TFT_BLACK
  );

  productText(
    "CONNECTING WI-FI",
    180,
    150,
    TFT_WHITE,
    &fonts::Font2
  );

  uint32_t start =
    millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - start < 20000UL
  )
  {
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(
      "[WIFI] Connection FAILED."
    );

    return false;
  }

  Serial.println(
    "[WIFI] CONNECTED."
  );

  Serial.print(
    "[WIFI] IP: "
  );

  Serial.println(
    WiFi.localIP()
  );

  return true;
}

// ------------------------------------------------------------
// Animation storage / Xbox animation
// ------------------------------------------------------------

static bool initAnimationStorage()
{
  Serial.println(
    "[SD] Starting HSPI..."
  );

  // Explicitly deselect both SPI devices before SD initialisation.
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  Serial.println("[SD] TFT CS HIGH");
  Serial.println("[SD] SD CS HIGH");

  Serial.print("[SD] Heap before init: ");
  Serial.println(ESP.getFreeHeap());

  Serial.print("[MEM] Largest free block before SD init: ");
  Serial.println(ESP.getMaxAllocHeap());

  sdSPI.begin(
    SD_SCLK,
    SD_MISO,
    SD_MOSI,
    SD_CS
  );

  if (!SD.begin(
        SD_CS,
        sdSPI,
        SD_SPI_HZ
      ))
  {
    Serial.println(
      "[SD] ERROR: SD init failed."
    );

    Serial.print("[SD] Heap after failed init: ");
    Serial.println(ESP.getFreeHeap());

    return false;
  }

  Serial.println("[SD] SD.begin() OK");

  File file =
    SD.open(
      CBP_FILE,
      FILE_READ
    );

  if (!file)
  {
    Serial.println(
      "[SD] ERROR: Xbox CBP asset not found."
    );

    return false;
  }

  bool ok =
    loadCBPIndex(file);

  file.close();

  Serial.println(
    ok
      ? "[SD] Xbox CBP index loaded."
      : "[SD] ERROR: Xbox CBP index invalid."
  );

  Serial.print("[MEM] Heap after SD+index: ");
  Serial.println(ESP.getFreeHeap());

  Serial.print("[MEM] Largest free block after SD+index: ");
  Serial.println(ESP.getMaxAllocHeap());

  return ok;
}

static bool playXboxAnimation()
{
  File file =
    SD.open(
      CBP_FILE,
      FILE_READ
    );

  if (!file)
  {
    Serial.println(
      "[PLAYBACK] ERROR: CBP open failed."
    );

    return false;
  }

  Serial.println();
  Serial.println(
    "=========================================="
  );

  Serial.println(
    " XBOX BOOT ANIMATION START"
  );

  Serial.println(
    "=========================================="
  );

  for (
    uint32_t frame = 0;
    frame < MASTER_BOOT_END_FRAME && frame < cbpHeader.frameCount;
    ++frame
  )
  {
    // Restore the proven frame positioning used by the working
    // standalone player and other Library playback code.
    file.seek(indexTable[frame].offset);

    uint32_t frameUs = 0;
    uint32_t sdUs = 0;
    uint32_t decodeUs = 0;
    uint32_t dmaUs = 0;
    uint32_t stageUs = 0;

    if (!playFramePipelined(
          file,
          frame,
          frameUs,
          sdUs,
          decodeUs,
          dmaUs,
          stageUs
        ))
    {
      Serial.print(
        "[PLAYBACK] ERROR at frame "
      );

      Serial.println(
        frame
      );

      file.close();

      return false;
    }
  }

  file.close();

  Serial.println(
    "[PLAYBACK] Master boot section complete."
  );

  return true;
}

// ------------------------------------------------------------
// Factory reset / rear button
// ------------------------------------------------------------

static void factoryResetAndSetup()
{
  Serial.println(
    "[SETUP] FACTORY RESET REQUESTED"
  );

  setupPrefs.begin(
    "upcbadger",
    false
  );

  setupPrefs.clear();
  setupPrefs.end();

  savedSSID = "";
  savedPassword = "";
  savedGamerID = "";

  tft.fillScreen(
    TFT_BLACK
  );

  productText(
    "FACTORY RESET",
    180,
    150,
    TFT_WHITE,
    &fonts::Font2
  );

  productText(
    "STARTING SETUP",
    180,
    195,
    TFT_WHITE,
    &fonts::Font0
  );

  delay(1200);

  startConfigMode(false);
}

static void checkConfigButton()
{
  bool down =
    digitalRead(
      CONFIG_BUTTON_PIN
    ) == LOW;

  if (down)
  {
    if (!configButtonLatched)
    {
      configButtonLatched = true;
      configButtonDownMs = millis();
    }

    return;
  }

  if (!configButtonLatched)
    return;

  uint32_t held =
    millis() -
    configButtonDownMs;

  configButtonLatched = false;
  configButtonDownMs = 0;

  if (held >= FACTORY_RESET_HOLD_MS)
  {
    factoryResetAndSetup();
    return;
  }

  if (held >= CONFIG_HOLD_MS)
  {
    startConfigMode(false);
  }
}

// ------------------------------------------------------------
// Config-mode startup
// ------------------------------------------------------------

static void startConfigMode(bool preserveDisplay)
{
  configMode = true;
  preserveSetupDisplay = preserveDisplay;

  Serial.println();
  Serial.println(
    "=========================================="
  );

  Serial.println(
    " ConsoleBadger CONFIGURATION MODE"
  );

  Serial.println(
    "=========================================="
  );

  Serial.println(
    "[AP] Starting Wi-Fi SoftAP..."
  );

  WiFi.mode(
    WIFI_AP
  );

  WiFi.softAPdisconnect(
    true
  );

  delay(100);

  IPAddress ip(
    192,
    168,
    4,
    1
  );

  IPAddress mask(
    255,
    255,
    255,
    0
  );

  bool configOK =
    WiFi.softAPConfig(
      ip,
      ip,
      mask
    );

  Serial.print(
    "[AP] softAPConfig: "
  );

  Serial.println(
    configOK
      ? "OK"
      : "FAILED"
  );

  bool apOK =
    WiFi.softAP(
      "ConsoleBadger",
      nullptr,
      6,
      false,
      1
    );

  Serial.print(
    "[AP] softAP start: "
  );

  Serial.println(
    apOK
      ? "OK"
      : "FAILED"
  );

  if (!apOK)
  {
    Serial.println(
      "[AP] ERROR: SoftAP could not start."
    );

    while (true)
      delay(1000);
  }

  Serial.print(
    "[AP] SSID: "
  );

  Serial.println(
    WiFi.softAPSSID()
  );

  Serial.print(
    "[AP] IP: "
  );

  Serial.println(
    WiFi.softAPIP()
  );

  Serial.print(
    "[AP] Client count: "
  );

  Serial.println(
    WiFi.softAPgetStationNum()
  );

  setupDNS.start(
    53,
    "*",
    ip
  );

  Serial.println(
    "[DNS] Captive DNS started."
  );

  setupServer.on(
    "/scan",
    HTTP_GET,
    handleSetupScan
  );

  setupServer.on(
    "/",
    HTTP_GET,
    handleSetupRoot
  );

  setupServer.on(
    "/save",
    HTTP_POST,
    handleSetupSave
  );

  setupServer.on(
    "/generate_204",
    HTTP_GET,
    handleSetupRoot
  );

  setupServer.on(
    "/hotspot-detect.html",
    HTTP_GET,
    handleSetupRoot
  );

  setupServer.on(
    "/connecttest.txt",
    HTTP_GET,
    handleSetupRoot
  );

  setupServer.onNotFound([](){
    Serial.print(
      "[AP] HTTP unknown path: "
    );

    Serial.println(
      setupServer.uri()
    );

    setupServer.sendHeader(
      "Location",
      "/",
      true
    );

    setupServer.send(
      302,
      "text/plain",
      ""
    );
  });

  setupServer.begin();

  Serial.println(
    "[HTTP] Web server started on port 80."
  );

  Serial.println(
    "[AP] READY — connect phone to ConsoleBadger"
  );

  Serial.println(
    "[AP] Then use the captive portal."
  );

  Serial.println(
    "[AP] Mobile data should be OFF while testing."
  );

  Serial.println(
    "=========================================="
  );

  if (!preserveSetupDisplay)
    drawSetupScreen();
}

// ------------------------------------------------------------
// Retention protection
// ------------------------------------------------------------

// ------------------------------------------------------------
// Background home Wi-Fi
//
// IMPORTANT:
// The known-good SD + Xbox boot sequence is already complete before
// this function is called. This function never clears the screen,
// never blocks for 20 seconds and never touches the CBP player.
// ------------------------------------------------------------

static void startHomeWiFiBackground()
{
  if (!savedSSID.length())
  {
    Serial.println(
      "[WIFI] No saved network; staying offline."
    );

    return;
  }

  Serial.println();
  Serial.println(
    "[WIFI] V1.96: starting background home Wi-Fi after profile init..."
  );

  // V1.61 proven Wi-Fi startup sequence.
  // Deliberately do NOT force a driver shutdown or WIFI_OFF here.
  WiFi.mode(
    WIFI_STA
  );

  WiFi.setHostname(
    "consolebadger"
  );

  WiFi.begin(
    savedSSID.c_str(),
    savedPassword.c_str()
  );

  homeWiFiStarted = true;

  lastHomeWiFiStatus =
    WiFi.status();

  lastHomeWiFiAttemptMs = millis();

  Serial.println(
    "[WIFI] Background connection started."
  );
}

static void serviceHomeWiFi()
{
  if (!homeWiFiStarted)
    return;

  const wl_status_t status = WiFi.status();
  const uint32_t now = millis();

  if (status == WL_CONNECTED)
  {
    if (!homeWiFiReported)
    {
      Serial.println("[WIFI] CONNECTED.");

      badgerDiagCheckpoint(
        DIAG_WIFI_CONNECTED,
        "WIFI_CONNECTED"
      );

      Serial.print("[WIFI] IP: ");
      Serial.println(WiFi.localIP());

      homeWiFiReported = true;
      profileApiInitialSyncPending = true;
    }

    return;
  }

  // Non-blocking recovery for a connection attempt that has stalled.
  // Do not interfere with the TFT/SD animation while waiting.
  if (now - lastHomeWiFiAttemptMs >= 20000UL)
  {
    lastHomeWiFiAttemptMs = now;

    Serial.print("[WIFI] Connection retry; status=");
    Serial.println((int)status);

    // Retry without tearing down or reinitialising the Wi-Fi driver.
    WiFi.begin(
      savedSSID.c_str(),
      savedPassword.c_str()
    );
  }
}

// ------------------------------------------------------------
// Live Xbox profile synchronisation
//
// The setup GamerTag is the lookup key; live XBL data is the display source.
// ------------------------------------------------------------

static String urlEncodeProfileValue(
  const String &value
)
{
  String out;

  for (size_t i = 0; i < value.length(); ++i)
  {
    const char c = value[i];

    if (
      (c >= 'a' && c <= 'z') ||
      (c >= 'A' && c <= 'Z') ||
      (c >= '0' && c <= '9') ||
      c == '-' || c == '_' || c == '.' || c == '~'
    )
    {
      out += c;
    }
    else
    {
      const char hex[] = "0123456789ABCDEF";
      out += '%';
      out += hex[(c >> 4) & 0x0F];
      out += hex[c & 0x0F];
    }
  }

  return out;
}

static bool extractJsonString(
  const String &json,
  const char *key,
  String &out
)
{