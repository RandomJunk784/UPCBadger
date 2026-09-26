/*
  UPCBadger v1.49
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
// V1.49 TLS MEMORY RECOVERY TEST
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
  if (file.read(
        (uint8_t *)&cbpHeader,
        sizeof(cbpHeader)
      ) != sizeof(cbpHeader))
  {
    Serial.println("ERROR: CBP header read failed");
    return false;
  }

  if (memcmp(cbpHeader.magic, "CBP1", 4) != 0)
  {
    Serial.println("ERROR: CBP magic mismatch");
    return false;
  }

  if (cbpHeader.version != CBP_VERSION ||
      cbpHeader.codec != CBP_CODEC_PALETTE_LZ4)
  {
    Serial.println("ERROR: unsupported CBP codec/version");
    return false;
  }

  if (cbpHeader.width != WIDTH ||
      cbpHeader.height != HEIGHT)
  {
    Serial.println("ERROR: CBP geometry mismatch");
    return false;
  }

  if (cbpHeader.frameCount != FRAME_COUNT_EXPECTED)
  {
    Serial.println("ERROR: CBP frame count mismatch");
    return false;
  }

  if (cbpHeader.displayFrameBytes !=
      DISPLAY_FRAME_BYTES)
  {
    Serial.println("ERROR: CBP display bytes mismatch");
    return false;
  }

  if (cbpHeader.codedFrameBytes !=
      FRAME_PIXELS)
  {
    Serial.println("ERROR: CBP coded bytes mismatch");
    return false;
  }

  if (cbpHeader.blockIndexBytes !=
      BLOCK_INDEX_BYTES)
  {
    Serial.println("ERROR: CBP block size mismatch");
    return false;
  }

  if (!(cbpHeader.flags & DIRECT_DMA_FLAG))
  {
    Serial.println("ERROR: CBP is not DIRECT-DMA");
    return false;
  }

  if (!(cbpHeader.flags & PALETTE_FLAG))
  {
    Serial.println("ERROR: CBP is not palette encoded");
    return false;
  }

  if (cbpHeader.indexEntryBytes !=
      sizeof(CBPIndexEntry))
  {
    Serial.println("ERROR: CBP index entry mismatch");
    return false;
  }

  if (cbpHeader.indexOffset != CBP_HEADER_BYTES ||
      cbpHeader.dataOffset != CBP_DATA_OFFSET)
  {
    Serial.println("ERROR: CBP offsets mismatch");
    return false;
  }

  file.seek(cbpHeader.indexOffset);

  size_t indexBytes =
    cbpHeader.frameCount *
    sizeof(CBPIndexEntry);

  if (file.read(
        (uint8_t *)indexTable,
        indexBytes
      ) != indexBytes)
  {
    Serial.println("ERROR: CBP index read failed");
    return false;
  }

  uint64_t sum = 0;
  for (uint32_t i = 0;
       i < cbpHeader.frameCount;
       ++i)
  {
    sum +=
      indexTable[i].storedBytes;

  }

  return true;
}

// ============================================================
// LOAD FRAME PALETTE
// ============================================================

static bool loadPalette(StageReader &reader)
{
  if (!stageEnsure(reader, PALETTE_BYTES))
    return false;

  memcpy(
    palette565,
    stageBuffer + reader.beginPos,
    PALETTE_BYTES
  );

  reader.beginPos +=
    PALETTE_BYTES;

  if (reader.beginPos == reader.endPos)
  {
    reader.beginPos = 0;
    reader.endPos = 0;
  }
  else if (reader.beginPos >= (STAGE_BUFFER_BYTES / 2))
  {
    stageCompact(reader);
  }

  return true;
}

// ============================================================
// PREPARE NEXT CBP BLOCK
// ============================================================

static bool prepareNextBlock(
  StageReader &reader,
  uint8_t *destination,
  size_t &pixelCount,
  uint32_t &decodeUs
)
{
  if (!stageEnsure(
        reader,
        sizeof(CBPBlockHeader)
      ))
  {
    return false;
  }

  CBPBlockHeader bh;

  memcpy(
    &bh,
    stageBuffer + reader.beginPos,
    sizeof(bh)
  );

  if (bh.rawBytes == 0 ||
      bh.rawBytes > BLOCK_INDEX_BYTES ||
      bh.storedBytes == 0 ||
      bh.storedBytes > BLOCK_INDEX_BYTES)
  {
    Serial.println("ERROR: invalid CBP block");
    return false;
  }

  size_t fullBytes =
    sizeof(CBPBlockHeader) +
    (size_t)bh.storedBytes;

  if (!stageEnsure(reader, fullBytes))
    return false;

  const uint8_t *src =
    stageBuffer +
    reader.beginPos +
    sizeof(CBPBlockHeader);

  uint32_t startDecode = micros();

  size_t indexBytesWritten = 0;

  if (bh.flags & 0x01)
  {
    if (bh.storedBytes != bh.rawBytes)
      return false;

    memcpy(
      destination,
      src,
      bh.rawBytes
    );

    indexBytesWritten =
      bh.rawBytes;
  }
  else
  {
    if (!lz4DecompressBlock(
          src,
          bh.storedBytes,
          destination,
          16384,
          bh.rawBytes,
          indexBytesWritten
        ))
    {
      return false;
    }
  }

  // The index bytes are at the start of the same 32 KiB DMA buffer.
  // Expand backwards in-place to RGB565 so the source indices are
  // never overwritten before they are consumed.
  uint16_t *pixels =
    reinterpret_cast<uint16_t *>(destination);

  for (size_t i = indexBytesWritten;
       i > 0;
       --i)
  {
    size_t n = i - 1;

    uint8_t index =
      destination[n];

    pixels[n] =
      palette565[index];
  }

  pixelCount =
    indexBytesWritten;

  decodeUs +=
    micros() - startDecode;

  reader.beginPos += fullBytes;

  if (reader.beginPos == reader.endPos)
  {
    reader.beginPos = 0;
    reader.endPos = 0;
  }
  else if (reader.beginPos >= (STAGE_BUFFER_BYTES / 2))
  {
    stageCompact(reader);
  }

  return true;
}

// ============================================================
// PIPELINED FRAME
// ============================================================

static bool playFramePipelined(
  File &file,
  uint32_t frameIndex,
  uint32_t &frameUs,
  uint32_t &sdUs,
  uint32_t &decodeUs,
  uint32_t &dmaUs,
  uint32_t &stageUs
)
{
  const CBPIndexEntry &entry =
    indexTable[frameIndex];

  StageReader reader;

  reader.file = &file;
  reader.beginPos = 0;
  reader.endPos = 0;

  reader.fileRemaining =
    entry.storedBytes;

  uint8_t *current =
    dmaBufferA;

  uint8_t *next =
    dmaBufferB;

  size_t currentPixels = 0;
  size_t nextPixels = 0;

  uint32_t frameStart =
    micros();

  // Palette comes first in every frame.
  if (!loadPalette(reader))
    return false;

  // Decode the first block before starting DMA.
  if (!prepareNextBlock(
        reader,
        current,
        currentPixels,
        decodeUs
      ))
  {
    return false;
  }

  tft.startWrite();

  // Single 330x350 address window.
  // The 18-pixel side margins on the 360x360 panel are never sent.
  tft.setAddrWindow(
    WINDOW_X,
    WINDOW_Y,
    WIDTH,
    HEIGHT
  );

  uint32_t pixelsSent = 0;

  const uint16_t blockCount =
    entry.blockCount;

  if (blockCount == 0)
  {
    tft.endWrite();
    return false;
  }

  for (uint16_t block = 0;
       block < blockCount;
       ++block)
  {
    tft.writePixelsDMA(
      reinterpret_cast<uint16_t *>(current),
      currentPixels,
      false
    );

    bool haveNext = false;

    if (block + 1 < blockCount)
    {
      haveNext =
        prepareNextBlock(
          reader,
          next,
          nextPixels,
          decodeUs
        );

      if (!haveNext)
      {
        uint32_t waitStart =
          micros();

        tft.waitDMA();

        dmaUs +=
          micros() - waitStart;

        tft.endWrite();
        return false;
      }
    }

    uint32_t waitStart =
      micros();

    tft.waitDMA();

    dmaUs +=
      micros() - waitStart;

    pixelsSent +=
      currentPixels;

    if (!haveNext)
      break;

    uint8_t *tmp =
      current;

    current = next;
    next = tmp;

    currentPixels =
      nextPixels;

    nextPixels = 0;
  }

  tft.endWrite();

  frameUs =
    micros() - frameStart;

  if (reader.fileRemaining != 0)
  {
    Serial.println(
      "ERROR: CBP frame not fully consumed"
    );

    return false;
  }

  if (reader.beginPos != reader.endPos)
  {
    Serial.println(
      "ERROR: CBP stage data not exhausted"
    );

    return false;
  }

  if (pixelsSent != FRAME_PIXELS)
  {
    Serial.print(
      "ERROR: pixels sent "
    );
    Serial.print(pixelsSent);
    Serial.print(" expected ");
    Serial.println(FRAME_PIXELS);
    return false;
  }

  sdUs =
    (uint32_t)reader.sdUs;

  stageUs =
    (uint32_t)reader.stageUs;

  return true;
}



// ============================================================
// CONSOLEBADGER PRODUCT LAYER
// KNOWN-GOOD BOOT -> SETUP CHECK -> GAMER PAGE -> WIFI
// ============================================================

#define CONFIG_BUTTON_PIN 32
#define CONFIG_HOLD_MS 3000UL
#define FACTORY_RESET_HOLD_MS 15000UL

// Forward declaration before early callers.
static void startConfigMode(bool preserveDisplay);


enum ConsoleMode : uint8_t
{
  MODE_XBOX = 0,
  MODE_PLAYSTATION = 1,
  MODE_NINTENDO = 2
};

static ConsoleMode consoleMode = MODE_XBOX;

static WebServer setupServer(80);
static DNSServer setupDNS;
static Preferences setupPrefs;

static String savedSSID;
static String savedPassword;
static String savedGamerID;

static bool configMode = false;
static bool preserveSetupDisplay = false;
static bool configButtonLatched = false;
static uint32_t configButtonDownMs = 0;


// ------------------------------------------------------------
// Background product services.
// These start only AFTER the known-good Xbox boot has completed.
// ------------------------------------------------------------

#define PROFILE_SYNC_INTERVAL_MS (60UL * 60UL * 1000UL)

static bool productProfileActive = false;
static bool homeWiFiStarted = false;
static bool homeWiFiReported = false;
static wl_status_t lastHomeWiFiStatus = WL_NO_SHIELD;
static uint32_t lastProfileSyncMs = 0;
static bool profileApiInitialSyncPending = false;
static bool profileApiAvailable = false;
static String liveGamerID;
static String liveXUID;
static uint32_t liveGamerScore = 0;
static String liveGamerPicURL;

// ------------------------------------------------------------
// Xbox profile / live-data state.
// V1.46 uses OpenXBL for a controlled live-profile test.
// The real API key lives only in local Secrets.h.
// ------------------------------------------------------------

static uint16_t productRGB565(
  uint8_t r,
  uint8_t g,
  uint8_t b
)
{
  return (uint16_t)(
    ((r & 0xF8) << 8) |
    ((g & 0xFC) << 3) |
    (b >> 3)
  );
}

static uint16_t productScale565(
  uint16_t c,
  uint8_t amount
)
{
  if (amount == 255)
    return c;

  uint32_t r = (c >> 11) & 0x1F;
  uint32_t g = (c >> 5) & 0x3F;
  uint32_t b = c & 0x1F;

  r = (r * amount + 127) / 255;
  g = (g * amount + 127) / 255;
  b = (b * amount + 127) / 255;

  return (uint16_t)(
    (r << 11) |
    (g << 5) |
    b
  );
}

static uint16_t xboxGreen(uint8_t a = 255)
{
  return productScale565(
    productRGB565(0, 255, 45),
    a
  );
}

static uint16_t xboxGreen2(uint8_t a = 255)
{
  return productScale565(
    productRGB565(0, 175, 35),
    a
  );
}

static uint16_t xboxGreen3(uint8_t a = 255)
{
  return productScale565(
    productRGB565(0, 85, 18),
    a
  );
}

static uint16_t productWhite(uint8_t a = 255)
{
  return productScale565(
    productRGB565(215, 225, 220),
    a
  );
}

static String gamerInitials(String value)
{
  value.trim();
  value.toUpperCase();

  if (value.length() == 0)
    return "X";

  if (value.length() > 2)
    value = value.substring(0, 2);

  return value;
}

static void productText(
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
  const CBPIndexEntry &entry =
    indexTableLocal[frameIndex];

  // The reader is frame-local. Always position the File at the exact
  // indexed payload for this frame instead of relying on sequential EOF/
  // frame-wrap behaviour.
  if (!file.seek(entry.offset))
  {
    Serial.println(
      "[PROFILE] CBP frame seek failed."
    );
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
  uint32_t frameStart = micros();

  if (!loadPalette(reader))
    return false;

  if (!prepareNextBlock(
        reader,
        current,
        currentPixels,
        decodeUs
      ))
  {
    return false;
  }

  tft.startWrite();

  tft.setAddrWindow(
    0,
    displayY,
    frameWidth,
    frameHeight
  );

  uint32_t pixelsSent = 0;
  const uint16_t blockCount = entry.blockCount;

  for (uint16_t block = 0; block < blockCount; ++block)
  {
    tft.writePixelsDMA(
      reinterpret_cast<uint16_t *>(current),
      currentPixels,
      false
    );

    bool haveNext = false;

    if (block + 1 < blockCount)
    {
      haveNext =
        prepareNextBlock(
          reader,
          next,
          nextPixels,
          decodeUs
        );

      if (!haveNext)
      {
        uint32_t waitStart = micros();
        tft.waitDMA();
        dmaUs += micros() - waitStart;
        tft.endWrite();
        return false;
      }
    }

    uint32_t waitStart = micros();
    tft.waitDMA();
    dmaUs += micros() - waitStart;

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

  if (
    reader.fileRemaining != 0 ||
    reader.beginPos != reader.endPos ||
    pixelsSent != frameWidth * frameHeight
  )
  {
    Serial.println(
      "[PROFILE] Profile animation frame validation failed."
    );
    return false;
  }

  sdUs = (uint32_t)reader.sdUs;
  stageUs = (uint32_t)reader.stageUs;
  return true;
}

static bool renderProfileAnimationFrame(uint32_t frameIndex)
{
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

static void drawProfileWifiIcon(
  bool connected,
  uint8_t brightness,
  int cx,
  int cy
)
{
  (void)connected;
  (void)brightness;
  const uint16_t c = profileWhite(255);

  for (int band = 0; band < 3; ++band)
  {
    float radius = 10.0f + band * 6.0f;
    float start = -2.45f;
    float end = -0.70f;
    int segments = 8;

    int px = cx + (int)(cosf(start) * radius);
    int py = cy + (int)(sinf(start) * radius);

    for (int i = 1; i <= segments; ++i)
    {
      float a = start + (end - start) * ((float)i / segments);
      int nx = cx + (int)(cosf(a) * radius);
      int ny = cy + (int)(sinf(a) * radius);

      tft.drawLine(px, py, nx, ny, c);
      px = nx;
      py = ny;
    }
  }

  tft.fillCircle(cx, cy + 5, 3, c);
}

static void drawProfileOverlay(
  uint8_t brightness
)
{
  const int cx = 180;
  const uint16_t white = profileWhite(brightness);
  // Gamer ID sits in the lower black/gloss region. Use Font2 normally;
  // fall back to the smaller Font0 if a long username approaches the sides.
  const String gamerId =
    liveGamerID.length()
      ? liveGamerID
      : (savedGamerID.length() ? savedGamerID : "PLAYER");

  const lgfx::IFont *idFont = &fonts::Font2;
  tft.setFont(idFont);

  if (tft.textWidth(gamerId) > 300)
    idFont = &fonts::Font0;

  profileText(
    gamerId,
    cx,
    100,
    white,
    idFont
  );

  // The centre remains clear for the future live Xbox gamerpic.

  // White status indicators sit left/right of the future gamerpic well.
  const int statusY = 180;
  const int onlineDotX = 78;
  const int onlineTextX = 103;
  const int wifiIconX = 270;

  tft.fillCircle(onlineDotX, statusY, 4, white);

  profileText(
    "ONLINE",
    onlineTextX,
    statusY,
    white,
    &fonts::Font0,
    textdatum_t::middle_center
  );

  // Beacon retained; Wi-Fi text intentionally removed to reduce visual clutter.
  drawProfileWifiIcon(
    WiFi.status() == WL_CONNECTED,
    brightness,
    wifiIconX,
    statusY
  );
}


// ------------------------------------------------------------
// Profile band sender
// ------------------------------------------------------------
// Sends only the intersection of one decoded CBP block with the
// central full-width profile energy band. Full rows are grouped into
// one DMA transfer; only edge fragments need a one-row transfer.

static void drawProfileStatusOnly()
{
  const uint16_t white = profileWhite(255);
  const int statusY = 180;
  const int onlineDotX = 78;
  const int onlineTextX = 103;
  const int wifiIconX = 270;

  tft.fillCircle(onlineDotX, statusY, 4, white);

  profileText(
    "ONLINE",
    onlineTextX,
    statusY,
    white,
    &fonts::Font0,
    textdatum_t::middle_center
  );

  drawProfileWifiIcon(
    WiFi.status() == WL_CONNECTED,
    255,
    wifiIconX,
    statusY
  );
}

static bool drawProfileScreen(
  uint8_t brightness
)
{
  // Master asset: boot frames 0..103, Gamer ID loop frames 104..180.
  profileAnimFrame = MASTER_LOOP_START_FRAME;

  if (!renderProfileAnimationFrame(profileAnimFrame))
  {
    tft.fillScreen(TFT_BLACK);

    profileText(
      "PROFILE ANIMATION ERROR",
      180,
      170,
      profileWhite(brightness),
      &fonts::Font0
    );

    return false;
  }

  drawProfileOverlay(
    brightness
  );

  profileAnimFrame = MASTER_LOOP_START_FRAME + 1UL;
  if (profileAnimFrame >= MASTER_LOOP_END_FRAME)
    profileAnimFrame = MASTER_LOOP_START_FRAME;

  profileAnimNextUs =
    micros() + PROFILE_ANIM_FRAME_INTERVAL_US;

  return true;
}

static void serviceProfileAnimation()
{
  if (!productProfileActive || !profileAnimReady)
    return;

  uint32_t now = micros();

  if ((int32_t)(now - profileAnimNextUs) < 0)
    return;

  const uint32_t frameCount =
    profileAnimHeader.frameCount;

  if (frameCount == 0)
    return;

  uint32_t frame = profileAnimFrame;
  if (frame < MASTER_LOOP_START_FRAME || frame >= MASTER_LOOP_END_FRAME)
    frame = MASTER_LOOP_START_FRAME;

  if (!renderProfileAnimationFrame(frame))
  {
    Serial.println(
      "[PROFILE] Full-screen animation frame update failed."
    );
    profileAnimNextUs =
      now + PROFILE_ANIM_FRAME_INTERVAL_US;
    return;
  }

  // The master background redraw covers the whole canvas, so restore the live
  // profile overlay immediately after each frame.
  drawProfileOverlay(255);

  frame++;
  if (frame >= MASTER_LOOP_END_FRAME)
    frame = MASTER_LOOP_START_FRAME;
  profileAnimFrame = frame;

  // Keep cadence tied to the prior deadline. If this frame overruns,
  // the next frame is already due and must not inherit another full
  // 83.333 ms wait. This specifically prevents a pause at frame 96 -> 0.
  uint32_t next =
    profileAnimNextUs + PROFILE_ANIM_FRAME_INTERVAL_US;

  if ((int32_t)(next - now) <= 0)
    next = now;

  profileAnimNextUs = next;
}

// ------------------------------------------------------------
// Theme-neutral setup screen
// ------------------------------------------------------------

static void drawSetupScreen()
{
  tft.fillScreen(TFT_BLACK);

  const int cx = 180;

  tft.setTextDatum(
    textdatum_t::middle_center
  );

  tft.setFont(
    &fonts::Font0
  );

  tft.setTextSize(2);

  // Keep the physical setup screen calm and simple.
  tft.setTextColor(
    TFT_WHITE,
    TFT_BLACK
  );

  tft.drawString(
    "ConsoleBadger Setup",
    cx,
    95
  );

  tft.drawString(
    "Find ConsoleBadger Wifi and",
    cx,
    150
  );

  tft.drawString(
    "connect using your phone",
    cx,
    195
  );

  tft.drawString(
    "(Turn OFF mobile data)",
    cx,
    255
  );

  tft.setTextSize(1);
}

// ------------------------------------------------------------
// Browser setup UI
// ------------------------------------------------------------

static const char SETUP_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>ConsoleBadger Setup</title>
<style>
body{margin:0;background:#000;color:#fff;font-family:Arial,sans-serif}
.wrap{max-width:430px;margin:auto;padding:20px}
.card{background:#111;border:1px solid #fff;border-radius:18px;padding:22px}
h1{margin:0;color:#fff;font-size:30px}
p,label{color:#fff}
label{display:block;margin-top:16px;font-size:14px}
input,select{
 box-sizing:border-box;width:100%;padding:14px;margin-top:7px;
 border-radius:10px;border:1px solid #fff;background:#000;color:#fff;font-size:16px
}
button{
 width:100%;padding:14px;margin-top:18px;border:1px solid #fff;
 border-radius:10px;background:#fff;color:#000;font-weight:bold;font-size:16px
}
button.secondary{background:#000;color:#fff}
#networks{margin-top:12px}
.net{
 padding:12px;border:1px solid #fff;border-radius:10px;
 margin-top:8px;background:#000;cursor:pointer
}
.net b{color:#fff}
.meta,.status,.small,.mode-note{color:#bbb;font-size:12px}
.status{min-height:18px;margin-top:10px}
.small{margin-top:18px}
</style>
</head>
<body>
<div class="wrap">
<div class="card">

<h1>ConsoleBadger</h1>
<p>Configure your badge.</p>

<label>Console mode
<select name="mode" form="setupForm">
<option value="0">Xbox</option>
<option value="1" disabled>PlayStation (coming later)</option>
<option value="2" disabled>Nintendo (coming later)</option>
</select>
</label>

<button class="secondary" type="button" onclick="scanWiFi()">
SCAN FOR WI-FI NETWORKS
</button>

<div id="status" class="status">
Tap scan to find nearby networks.
</div>

<div id="networks"></div>

<form id="setupForm" method="POST" action="/save">

<label>Wi-Fi network
<input id="ssid" name="ssid" maxlength="64" autocomplete="off" required>
</label>

<label>Wi-Fi password
<input name="password" type="password" maxlength="64" autocomplete="off">
</label>

<label>Gamer ID
<input name="gamertag" maxlength="32" autocomplete="off" required>
</label>

<button>SAVE &amp; CONNECT</button>

</form>

<div class="small">
Choose Xbox for this prototype, select your Wi-Fi, then enter the password and Gamer ID.
</div>

</div>
</div>

<script>
async function scanWiFi(){
 const status=document.getElementById('status');
 const list=document.getElementById('networks');

 status.textContent='Scanning nearby Wi-Fi...';
 list.innerHTML='';

 try{
  const r=await fetch('/scan');
  if(!r.ok) throw new Error('scan failed');

  const nets=await r.json();

  if(!nets.length){
   status.textContent='No networks found. Enter the Wi-Fi name manually.';
   return;
  }

  status.textContent=nets.length+' network'+(nets.length===1?'':'s')+' found. Tap yours.';

  nets.forEach(n=>{
   const row=document.createElement('div');
   row.className='net';

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
    "[AP] Starting Wi-Fi SoftAP..."  );

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
    "[WIFI] Starting background home Wi-Fi..."
  );

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

  Serial.println(
    "[WIFI] Background connection started."
  );
}

static void serviceHomeWiFi()
{
  if (!homeWiFiStarted)
    return;

  wl_status_t status =
    WiFi.status();

  if (status == lastHomeWiFiStatus)
    return;

  lastHomeWiFiStatus =
    status;

  if (status == WL_CONNECTED)
  {
    Serial.println(
      "[WIFI] CONNECTED."
    );

    Serial.print(
      "[WIFI] IP: "
    );

    Serial.println(
      WiFi.localIP()
    );

    homeWiFiReported = true;
    profileApiInitialSyncPending = true;

    // Update only the profile status line, using the existing
    // profile renderer. No new display system is introduced.
    if (productProfileActive)
    {
      drawProfileStatusOnly();
    }

    return;
  }

  if (homeWiFiReported)
  {
    Serial.print(
      "[WIFI] Lost connection. status="
    );

    Serial.println(
      (int)status
    );

    homeWiFiReported = false;

    if (productProfileActive)
    {
      drawProfileStatusOnly();
    }
  }
}

// ------------------------------------------------------------
// Profile synchronisation hook
//
// The external profile provider is intentionally NOT implemented yet.
// This is the safe placeholder that proves the product scheduler works.
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
  const String needle = String("\"") + key + "\"";
  int keyPos = json.indexOf(needle);

  if (keyPos < 0)
    return false;

  int colon = json.indexOf(':', keyPos + needle.length());
  if (colon < 0)
    return false;

  int start = colon + 1;
  while (start < (int)json.length() &&
         (json[start] == ' ' || json[start] == '\t' ||
          json[start] == '\r' || json[start] == '\n'))
  {
    ++start;
  }

  if (start >= (int)json.length() || json[start] != '"')
    return false;

  ++start;
  String value;
  bool escaped = false;

  for (int i = start; i < (int)json.length(); ++i)
  {
    const char c = json[i];

    if (escaped)
    {
      if (c == '"' || c == '\\' || c == '/')
        value += c;
      else if (c == 'n')
        value += '\n';
      else if (c == 'r')
        value += '\r';
      else if (c == 't')
        value += '\t';
      else
        value += c;

      escaped = false;
      continue;
    }

    if (c == '\\')
    {
      escaped = true;
      continue;
    }

    if (c == '"')
    {
      out = value;
      return true;
    }

    value += c;
  }

  return false;
}

static bool extractJsonUInt32(
  const String &json,
  const char *key,
  uint32_t &out
)
{
  const String needle = String("\"") + key + "\"";
  int keyPos = json.indexOf(needle);

  if (keyPos < 0)
    return false;

  int colon = json.indexOf(':', keyPos + needle.length());
  if (colon < 0)
    return false;

  int start = colon + 1;
  while (start < (int)json.length() &&
         (json[start] == ' ' || json[start] == '\t' ||
          json[start] == '\r' || json[start] == '\n'))
  {
    ++start;
  }

  unsigned long value = 0;
  bool haveDigit = false;

  for (int i = start; i < (int)json.length(); ++i)
  {
    const char c = json[i];

    if (c < '0' || c > '9')
      break;

    value = value * 10UL + (unsigned long)(c - '0');
    haveDigit = true;
  }

  if (!haveDigit)
    return false;

  out = (uint32_t)value;
  return true;
}

static void printTlsHeap(const char *label)
{
  Serial.print("[MEM] ");
  Serial.print(label);
  Serial.print(" free=");
  Serial.print(ESP.getFreeHeap());
  Serial.print(" largest=");
  Serial.println(ESP.getMaxAllocHeap());
}

static bool suspendAnimationForTLS()
{
  printTlsHeap("Before TLS suspension");

  if (profileAnimFile)
  {
    profileAnimFile.close();
    Serial.println("[MEM] Profile CBP file closed for TLS.");
  }

  if (stageBuffer)
  {
    free(stageBuffer);
    stageBuffer = nullptr;
    Serial.println("[MEM] 32 KiB staging buffer released for TLS.");
  }

  printTlsHeap("After TLS suspension");
  return true;
}

static bool restoreAnimationAfterTLS()
{
  if (!stageBuffer)
  {
    stageBuffer =
      (uint8_t *)malloc(STAGE_BUFFER_BYTES);

    if (!stageBuffer)
    {
      Serial.println("[MEM] ERROR: could not restore staging buffer after TLS.");
      return false;
    }

    Serial.println("[MEM] 32 KiB staging buffer restored.");
  }

  if (!profileAnimFile)
  {
    profileAnimFile =
      SD.open(PROFILE_ANIM_FILE, FILE_READ);

    if (!profileAnimFile)
    {
      Serial.println("[PROFILE] ERROR: could not reopen master CBP after TLS.");
      profileAnimReady = false;
      return false;
    }

    if (!validateCBPIndex(
          profileAnimFile,
          profileAnimHeader,
          profileAnimIndex,
          PROFILE_ANIM_MAX_FRAMES,
          PROFILE_ANIM_WIDTH,
          PROFILE_ANIM_HEIGHT,
          "Base animation restore"
        ))
    {
      profileAnimFile.close();
      profileAnimReady = false;
      Serial.println("[PROFILE] ERROR: master CBP validation failed after TLS.");
      return false;
    }

    profileAnimReady = true;
    Serial.println("[PROFILE] Master animation file restored after TLS.");
  }

  printTlsHeap("After TLS restore");
  return true;
}

static bool syncProfileNow()
{
  Serial.println();
  Serial.println("[NET] V1.49 TLS memory-recovery diagnostic.");

  if (strlen(UPCBADGER_OPENXBL_API_KEY) == 0)
  {
    Serial.println("[NET] NO KEY: add your key to local Secrets.h.");
    return false;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("[NET] Wi-Fi is not connected.");
    return false;
  }

  if (!suspendAnimationForTLS())
    return false;

  // ----------------------------------------------------------
  // STEP 1 — DNS
  // ----------------------------------------------------------
  IPAddress resolved;
  Serial.println("[NET] DNS lookup: api.xbl.io");

  if (!WiFi.hostByName("api.xbl.io", resolved))
  {
    Serial.println("[NET] DNS FAILED.");
    restoreAnimationAfterTLS();
    return false;
  }

  Serial.print("[NET] DNS OK: ");
  Serial.println(resolved);

  // ----------------------------------------------------------
  // STEP 2 — plain TCP controls
  // ----------------------------------------------------------
  {
    WiFiClient plain80;
    plain80.setTimeout(5000);

    Serial.println("[NET] Raw TCP probe: api.xbl.io:80");

    if (!plain80.connect(resolved, 80))
    {
      Serial.println("[NET] TCP port 80 FAILED.");
    }
    else
    {
      Serial.println("[NET] TCP port 80 OK.");
      plain80.stop();
    }
  }

  {
    WiFiClient plain443;
    plain443.setTimeout(5000);

    Serial.print("[NET] Raw TCP probe: ");
    Serial.print(resolved);
    Serial.println(":443");

    if (!plain443.connect(resolved, 443))
    {
      Serial.print("[NET] TCP 443 FAILED. errno=");
      Serial.println(errno);
      plain443.stop();
      restoreAnimationAfterTLS();
      return false;
    }

    Serial.println("[NET] TCP 443 OK.");
    plain443.stop();
  }

  // ----------------------------------------------------------
  // STEP 3 — TLS handshake with SNI + detailed mbedTLS error
  // ----------------------------------------------------------
  {
    WiFiClientSecure secure;
    secure.setInsecure();
    secure.setHandshakeTimeout(8);

    Serial.println("[NET] TLS handshake: api.xbl.io:443");

    if (!secure.connect("api.xbl.io", 443))
    {
      char errorBuffer[160] = {0};
      const int errorCode =
        secure.lastError(errorBuffer, sizeof(errorBuffer));

      Serial.println("[NET] TLS CONNECT FAILED.");
      Serial.print("[NET] TLS lastError code: ");
      Serial.println(errorCode);
      Serial.print("[NET] TLS lastError text: ");
      Serial.println(errorBuffer);
      secure.stop();
      restoreAnimationAfterTLS();
      return false;
    }

    Serial.println("[NET] TLS CONNECT OK.");
    secure.stop();
  }

  // ----------------------------------------------------------
  // STEP 4 — exact OpenXBL account call already proven from PC
  // ----------------------------------------------------------
  const String url =
    "https://api.xbl.io/v2/account";

  Serial.println("[API] GET /v2/account");

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.setConnectTimeout(5000);
  http.setTimeout(7000);

  if (!http.begin(client, url))
  {
    Serial.println("[API] HTTP begin failed.");
    restoreAnimationAfterTLS();
    return false;
  }

  http.addHeader("X-Authorization", UPCBADGER_OPENXBL_API_KEY);
  http.addHeader("Accept", "application/json");

  const int httpCode = http.GET();

  Serial.print("[API] HTTP status: ");
  Serial.println(httpCode);

  if (httpCode != HTTP_CODE_OK)
  {
    if (httpCode > 0)
    {
      String errorBody = http.getString();
      Serial.print("[API] Response: ");
      Serial.println(errorBody.substring(0, 320));
    }
    else
    {
      Serial.print("[API] Transport error: ");
      Serial.println(http.errorToString(httpCode));
    }

    http.end();
    restoreAnimationAfterTLS();
    return false;
  }

  const String body = http.getString();
  http.end();

  Serial.println("[API] ACCOUNT REQUEST SUCCESS.");
  Serial.print("[API] Response preview: ");
  Serial.println(body.substring(0, 360));

  Serial.println("[NET] V1.49 transport path PASS.");
  restoreAnimationAfterTLS();
  return true;

}

static void serviceProfileSync()
{
  if (!productProfileActive || !homeWiFiStarted)
    return;

  uint32_t now = millis();

  if (WiFi.status() != WL_CONNECTED)
    return;

  if (profileApiInitialSyncPending)
  {
    profileApiInitialSyncPending = false;

    const bool success = syncProfileNow();

    if (success)
    {
      lastProfileSyncMs = now;
    }
    else
    {
      lastProfileSyncMs = now;
    }

    return;
  }

  if (
    now - lastProfileSyncMs <
    PROFILE_SYNC_INTERVAL_MS
  )
  {
    return;
  }

  lastProfileSyncMs = now;
  syncProfileNow();
}

// ============================================================
// FINAL PRODUCT SETUP / LOOP
// ============================================================


void setup()
{
  Serial.begin(115200);
  delay(300);

  pinMode(
    CONFIG_BUTTON_PIN,
    INPUT_PULLUP
  );

  Serial.println();
  Serial.println(
    "=========================================="
  );

  Serial.println(
    " UPCBadger v1.49"
  );

  Serial.println(
    " KNOWN-GOOD BOOT -> PROFILE -> WIFI"
  );

  Serial.println(
    "=========================================="
  );

  // ==========================================================
  // KNOWN-GOOD PLAYER STARTUP
  //
  // DO NOT rearrange this section.
  // This is the startup sequence that has already proven that
  // the SD card can mount and the Xbox animation can play.
  // ==========================================================

  if (!tft.init())
  {
    Serial.println(
      "[TFT] ERROR: TFT init failed."
    );

    while (true)
      delay(1000);
  }

  tft.setRotation(0);
  tft.setColorDepth(16);
  tft.setSwapBytes(false);
  tft.initDMA();
  tft.fillScreen(TFT_BLACK);

  // ----------------------------------------------------------
  // KNOWN-GOOD SD INITIALISATION
  // ----------------------------------------------------------

  if (!initAnimationStorage())
  {
    tft.fillScreen(
      TFT_BLACK
    );

    productText(
      "SD ERROR",
      180,
      150,
      TFT_WHITE,
      &fonts::Font2
    );

    productText(
      "CHECK SD CARD",
      180,
      195,
      TFT_WHITE,
      &fonts::Font0
    );

    while (true)
      delay(1000);
  }

  // ----------------------------------------------------------
  // PLAYBACK STAGING
  //
  // Allocated AFTER SD initialisation and CBP index validation
  // (v1.14 change, kept). v1.16 also shrank the buffer itself from
  // 96 KiB to STAGE_BUFFER_BYTES (32 KiB) - see the comment at its
  // declaration for why 32 KiB is enough.
  // ----------------------------------------------------------
  stageBuffer =
    (uint8_t *)malloc(
      STAGE_BUFFER_BYTES
    );

  if (!stageBuffer)
  {
    Serial.println(
      "[MEM] ERROR: staging allocation failed after SD init."
    );

    Serial.print("[MEM] Free heap at failure: ");
    Serial.println(ESP.getFreeHeap());

    Serial.print("[MEM] Largest free block at failure: ");
    Serial.println(ESP.getMaxAllocHeap());

    tft.fillScreen(TFT_BLACK);

    productText(
      "MEMORY ERROR",
      180,
      150,
      TFT_WHITE,
      &fonts::Font2
    );

    while (true)
      delay(1000);
  }

  Serial.println(
    "[MEM] 32 KiB staging allocated after SD init."
  );

  // ----------------------------------------------------------
  // KNOWN-GOOD XBOX BOOT
  // ----------------------------------------------------------

  if (!playXboxAnimation())
  {
    tft.fillScreen(
      TFT_BLACK
    );

    productText(
      "BOOT ERROR",
      180,
      150,
      TFT_WHITE,
      &fonts::Font2
    );

    while (true)
      delay(1000);
  }

  // ==========================================================
  // EVERYTHING BELOW THIS LINE IS NEW PRODUCT LAYER
  // ==========================================================

  Serial.println();
  Serial.println(
    "[PRODUCT] Known-good Xbox boot finished."
  );

  // ----------------------------------------------------------
  // LOOP MARKER / NVS SETUP CHECK
  //
  // The master animation has reached its Gamer ID loop marker.
  // First-time units pause on that frame while the existing setup AP runs.
  // Configured units continue directly into the Gamer ID loop.
  // ----------------------------------------------------------

  if (!loadProfileAnimIndex())
  {
    tft.fillScreen(TFT_BLACK);

    profileText(
      "PROFILE ANIMATION ERROR",
      180,
      170,
      profileWhite(255),
      &fonts::Font0
    );

    while (true)
      delay(1000);
  }

  bool configured =
    loadProductSettings();

  if (!configured)
  {
    // Show and hold the exact Gamer ID loop-start frame while provisioning.
    if (!renderProfileAnimationFrame(MASTER_LOOP_START_FRAME))
    {
      Serial.println(
        "[PRODUCT] Could not render setup loop marker frame."
      );
    }
    else
    {
      profileAnimFile.close();
    }

    Serial.println(
      "[PRODUCT] Setup required. Pausing at Gamer ID loop marker."
    );

    startConfigMode(true);
    return;
  }

  if (digitalRead(CONFIG_BUTTON_PIN) == LOW)
  {
    Serial.println(
      "[PRODUCT] Setup required by config button."
    );

    startConfigMode(false);
    return;
  }

  Serial.print(
    "[PRODUCT] Gamer ID: "
  );

  Serial.println(
    savedGamerID
  );

  Serial.print(
    "[PRODUCT] Console: "
  );

  Serial.println(
    consoleMode == MODE_XBOX
      ? "XBOX"
      : "OTHER"
  );

  // ----------------------------------------------------------
  // GAMER ID SCREEN
  // ----------------------------------------------------------

  if (!loadProfileAnimIndex())
  {
    tft.fillScreen(TFT_BLACK);

    profileText(
      "PROFILE ANIMATION ERROR",
      180,
      170,
      profileWhite(255),
      &fonts::Font0
    );
    while (true)
      delay(1000);
  }

  productProfileActive = true;
  lastProfileSyncMs = millis();
  profileAnimNextUs = 0;

  if (!drawProfileScreen(255))
  {
    while (true)
      delay(1000);
  }

  Serial.println(
    "[PROFILE] Gamer ID screen active."
  );

  // ----------------------------------------------------------
  // BACKGROUND WI-FI
  //
  // Non-blocking. The Gamer ID screen stays visible while the
  // ESP32 quietly connects to the saved home network.
  // ----------------------------------------------------------

  startHomeWiFiBackground();

  Serial.println(
    "[PRODUCT] Normal operation started."
  );
}

void loop()
{
  // ----------------------------------------------------------
  // SETUP MODE
  // ----------------------------------------------------------

  if (configMode)
  {
    setupDNS.processNextRequest();
    setupServer.handleClient();

    static uint8_t lastClientCount = 255;

    uint8_t clients =
      WiFi.softAPgetStationNum();

    if (
      clients !=
      lastClientCount
    )
    {
      Serial.print(
        "[AP] Connected clients: "
      );

      Serial.println(
        clients
      );

      lastClientCount =
        clients;
    }

    delay(5);
    return;
  }

  // ----------------------------------------------------------
  // PRODUCT MODE
  // ----------------------------------------------------------

  checkConfigButton();

  serviceHomeWiFi();
  serviceProfileSync();
  serviceProfileAnimation();


  delay(5);
}


