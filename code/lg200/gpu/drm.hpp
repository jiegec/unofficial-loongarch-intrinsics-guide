#pragma once

// Minimal PS-carrier DRM submission path.
//
// This layer is deliberately opaque about shader semantics: the PS program
// arrives as assembled dwords and is written verbatim into the fragment
// shader BO. It does NOT import the ISA codec, the shader builder, or any
// per-case knowledge (no ../isa, ../shader, ../ops includes). Per-case
// isolation (one process per case) is the CALLER's responsibility; each
// submit_ps() call performs exactly one submission.

#include <cstdint>
#include <string>
#include <vector>

namespace lgpu::drm {

struct PsSubmitRequest {
  // Fully-assembled PS program dwords, little-endian. Every 8 bytes (two
  // dwords) encode one instruction; the bytes are written verbatim into the
  // fragment shader BO. Opaque to this layer.
  std::vector<std::uint32_t> shader_words;
  // Argument-BO preload dwords, written little-endian starting at byte
  // offset 0 of the argument BO (resource 0, GPU VA 0x100000000).
  std::vector<std::uint32_t> argument_dwords;
  // Readback size in bytes from the result region (the argument BO, where
  // the PS program writes its results). Clamped to one page (16 KiB).
  std::uint32_t result_bytes = 4096;
  // Fence-wait timeout in milliseconds.
  std::uint32_t timeout_ms = 3000;
  // Wrap the submission in GPU timestamp commands (opcode 0x13 begin before
  // the full-state object, end after the end command, results at
  // arg+0x3000/0x3008): report.ts_begin/ts_end carry the 64-bit counter.
  bool timestamped = false;
  // Optional GS-program dwords (assembled, little-endian, 16-byte-aligned).
  // When non-empty the submission installs the GS shader-info record
  // (shader_type 4, root = 0x100024000) and raises the GS-enable bit in the
  // stage-config word (0x24 -> 0x2c).
  std::vector<std::uint32_t> gs_words;
};

enum class PsStatus {
  Clean,     // submitted, completed, readback done, no fault, health OK
  Fault,     // completed but a GPU fault/guard-clobber was detected
  Timeout,   // fence wait expired
  Rejected,  // invalid request / pre-submit failure / no libdrm (dry-run)
};

struct PsSubmitReport {
  PsStatus status = PsStatus::Rejected;
  // Readback bytes: the first result_bytes bytes of the argument BO after
  // the submission completed.
  std::vector<std::uint8_t> result;
  // GPU timestamp counter values around the submission (valid only when the
  // request was timestamped; tick unit ~32 ns per the query probe floor).
  std::uint64_t ts_begin = 0;
  std::uint64_t ts_end = 0;
  // Human-readable detail on any failure (empty on Clean).
  std::string detail;
};

// Submits the PS program once, waits for completion with timeout, reads
// back the result region, and runs a post-submit health check.
PsSubmitReport submit_ps(const PsSubmitRequest& request);

// Submits a PS+GS program pair: the same flow as submit_ps with the GS
// stage installed (stage-config GS enable + GS shader-info record).
PsSubmitReport submit_gs(const PsSubmitRequest& request);


// Probe submission (conformance-style PS roots): builds the reviewed packet
// with the given PS const/constant/rsd roots and writes the argument BO from
// (byte-offset, dword) preload pairs; returns the arg readback.
PsSubmitReport submit_probe(
    const std::vector<std::uint32_t>& shader_words,
    const std::vector<std::pair<std::uint32_t, std::uint32_t>>& arg_preload,
    std::uint64_t const_buffer, std::uint64_t constant, std::uint64_t rsd,
    std::uint32_t result_bytes, std::uint32_t timeout_ms);

// Timestamp-command probe (experiment): an IB containing opcode 0x13
// timestamp begin/end pairs around (shape 1) or without (shape 0) the full
// state object, results written to arg+0x3000/0x3008.
PsSubmitReport submit_timestamp_probe(std::uint32_t shape,
                                      std::uint32_t timeout_ms,
                                      std::uint32_t fs_size = 0);

}  // namespace lgpu::drm
