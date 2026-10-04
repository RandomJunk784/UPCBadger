/*
  UPCBadger v1.47
  CHANGELOG
  ----------
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
// V1.47 NETWORK DIAGNOSTIC
// ------------------------------------------------------------
// Parent: V1.44 compile-repair firmware.
// Adds a controlled OpenXBL profile lookup for test account
// renaultman172. No real API key is stored in this source.
// Secrets live in local Secrets.h.
// ============================================================
#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Secrets.h"

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
    pc.dlen_16bit = false;
    pc.bus_shared = false;

    panel.config(pc);
    setPanel(&panel);
  }
};

LGFX tft;
SPIClass sdSPI(HSPI);

// ============================================================
// CBP STRUCTURES
// ============================================================

#pragma pack(push, 1)

struct CBPHeader
{
  char magic[4];
  uint16_t version;
  uint16_t codec;
  uint16_t width;
  uint16_t height;

  uint32_t fpsNum;
  uint32_t fpsDen;
  uint32_t frameCount;

  // Decoded display bytes per frame.
  uint32_t displayFrameBytes;

  // Encoded palette-index bytes per frame.
  uint32_t codedFrameBytes;

  // Maximum raw index bytes represented by one LZ4 block.
  uint32_t blockIndexBytes;

  uint16_t indexEntryBytes;
  uint16_t flags;

  uint32_t indexOffset;
  uint32_t dataOffset;
};

struct CBPIndexEntry
{
  uint32_t offset;
  uint32_t storedBytes;
  uint16_t blockCount;
  uint16_t flags;
  uint32_t crc32;
};

struct CBPBlockHeader
{
  uint32_t storedBytes;
  uint16_t rawBytes;
  uint8_t flags;
  uint8_t reserved;
};

#pragma pack(pop)

static_assert(sizeof(CBPHeader) <= CBP_HEADER_BYTES,
              "CBP header larger than 64 bytes");

static_assert(sizeof(CBPIndexEntry) == CBP_INDEX_ENTRY_BYTES,
              "CBP index entry size mismatch");

static_assert(sizeof(CBPBlockHeader) == 8,
              "CBP block header size mismatch");

// Guards against ever shrinking STAGE_BUFFER_BYTES below what a
// single stageEnsure() call can require. If this fires, the staging
// window is too small and reads will corrupt memory, not just fail
// cleanly - this must stay a compile-time check.
static_assert(
  STAGE_BUFFER_BYTES >= (sizeof(CBPBlockHeader) + BLOCK_INDEX_BYTES),
  "STAGE_BUFFER_BYTES too small for one CBP block"
);

static CBPHeader cbpHeader;
static CBPIndexEntry indexTable[FRAME_COUNT_EXPECTED];

// ============================================================
// STAGING READER
// ============================================================

struct StageReader
{
  File *file = nullptr;

  size_t beginPos = 0;
  size_t endPos = 0;

  uint32_t fileRemaining = 0;

  uint64_t sdUs = 0;
  uint64_t stageUs = 0;
};

static bool stageCompact(StageReader &r)
{
  if (r.beginPos == 0)
    return true;

  size_t leftover =
    r.endPos - r.beginPos;

  if (leftover > 0)
  {
    uint32_t start = micros();

    memmove(
      stageBuffer,
      stageBuffer + r.beginPos,
      leftover
    );

    r.stageUs +=
      micros() - start;
  }

  r.beginPos = 0;
  r.endPos = leftover;

  return true;
}

static bool stageFill(StageReader &r)
{
  if (r.fileRemaining == 0)
    return true;

  if (r.endPos == STAGE_BUFFER_BYTES)
  {
    if (!stageCompact(r))
      return false;
  }

  size_t freeSpace =
    STAGE_BUFFER_BYTES - r.endPos;

  size_t toRead =
    min(
      freeSpace,
      (size_t)r.fileRemaining
    );

  uint32_t start = micros();

  size_t got =
    r.file->read(
      stageBuffer + r.endPos,
      toRead
    );

  r.sdUs +=
    micros() - start;

  if (got != toRead)
  {
    Serial.print("ERROR: stage read ");
    Serial.print(got);
    Serial.print("/");
    Serial.println(toRead);
    return false;
  }

  r.endPos += got;
  r.fileRemaining -= got;

  return true;
}

static bool stageEnsure(
  StageReader &r,
  size_t wanted
)
{
  if (wanted > STAGE_BUFFER_BYTES)
    return false;

  while (
    (r.endPos - r.beginPos) < wanted
  )
  {
    if (r.fileRemaining == 0)
      return false;

    if (r.endPos == STAGE_BUFFER_BYTES)
    {
      if (!stageCompact(r))
        return false;
    }

    if (!stageFill(r))
      return false;
  }

  return true;
}

// ============================================================
// LZ4 BLOCK DECODER
// ============================================================

static bool lz4DecompressBlock(
  const uint8_t *src,
  size_t srcSize,
  uint8_t *dst,
  size_t dstCapacity,
  size_t expectedOutput,
  size_t &written
)
{
  size_t si = 0;
  size_t di = 0;

  while (si < srcSize)
  {
    uint8_t token = src[si++];

    size_t literalLength =
      token >> 4;

    if (literalLength == 15)
    {
      while (true)
      {
        if (si >= srcSize)
          return false;

        uint8_t extra = src[si++];
        literalLength += extra;

        if (extra != 255)
          break;
      }
    }

    if (si + literalLength > srcSize)
      return false;

    if (di + literalLength > dstCapacity)
      return false;

    if (literalLength > 0)
    {
      memcpy(
        dst + di,
        src + si,
        literalLength
      );

      si += literalLength;
      di += literalLength;
    }

    if (si >= srcSize)
      break;

    if (si + 2 > srcSize)
      return false;

    uint16_t offset =
      (uint16_t)src[si] |
      ((uint16_t)src[si + 1] << 8);

    si += 2;

    if (offset == 0 || offset > di)
      return false;

    size_t matchLength =
      (token & 0x0F) + 4;

    if ((token & 0x0F) == 15)
    {
      while (true)
      {
        if (si >= srcSize)
          return false;

        uint8_t extra = src[si++];
        matchLength += extra;

        if (extra != 255)
          break;
      }
    }

    if (di + matchLength > dstCapacity)
      return false;

    size_t matchPos =
      di - offset;

    for (size_t i = 0;
         i < matchLength;
         ++i)
    {
      dst[di++] =
        dst[matchPos + i];
    }
  }

  written = di;

  return written == expectedOutput;
}

// ============================================================
// CBP HEADER / INDEX
// ============================================================

static bool loadCBPIndex(File &file)
{
  file.seek(0);
