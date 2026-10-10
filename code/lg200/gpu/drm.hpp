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

// Compute (CPIPE) queue probe: allocate the ring, map it into the device VM,
// create a COMPUTE user queue over it and free it again. No dispatch is sent.
//
// This exists to answer, before any MCS stream is built, the questions a
// compute carrier can fail on that have nothing to do with its encoding:
// whether this session may create a COMPUTE queue at all, whether the kernel
// accepts a ring we mapped ourselves, and how large a queue it will take.
// It is deliberately separate from the dispatch path so a failure here cannot
// be confused with a malformed stream.
struct ComputeQueueReport {
  bool ok = false;
  std::uint32_t queue_id = 0;
  // Ring geometry actually requested, and the GPU VA handed to the kernel.
  std::uint64_t ring_va = 0;
  std::uint32_t ring_size = 0;
  // Human-readable detail on any failure (empty on success).
  std::string detail;
};

ComputeQueueReport compute_queue_probe(std::uint32_t ring_size = 64u * 1024u);

// Build the MCS stream and kernel descriptor for one compute dispatch, on the
// host, using the carrier's fixed GPU VA layout.  Nothing is submitted.
//
// This exists so the encoding can be inspected and diffed against the vendor's
// bytes before anything reaches the device: a stream that is wrong and a stream
// the firmware rejects look the same from the outside, and separating them
// saves a debug cycle per mistake.
struct ComputePlanReport {
  bool ok = false;
  // The 96-byte kernel descriptor, patched for this dispatch.
  std::vector<std::uint8_t> kd;
  // The MCS stream, padded to a 64-byte ring slot.
  std::vector<std::uint8_t> stream;
  // GPU VAs the plan assumes, echoed so the caller can stage matching BOs.
  std::uint64_t text_va = 0;
  std::uint64_t kd_va = 0;
  std::uint64_t kernarg_va = 0;
  std::uint64_t signal_va = 0;
  std::uint32_t num_sgprs = 0;
  std::uint32_t num_vgprs = 0;
  std::string detail;
};

// `text_size` sizes the kernel's code BO; group/grid are the launch geometry.
ComputePlanReport compute_plan(std::uint64_t text_size,
                               std::uint32_t group_x, std::uint32_t group_y,
                               std::uint32_t group_z, std::uint32_t grid_x,
                               std::uint32_t grid_y, std::uint32_t grid_z);

// One compute dispatch through the CPIPE user queue.
//
// The carrier owns every buffer: the caller supplies only the kernel's code and
// the arguments that follow the output pointer.  The output BO's GPU VA is
// written to kernarg offset 0, which is where a compiled kernel's first implicit
// argument lands (`load.s.flat.w2 s[0:1], s[6:7], zero, 0x0`).
struct ComputeSubmitReport {
  enum class Status {
    Clean,      // doorbell rung, completion signal retired, output read back
    Fault,      // completed but the output looks wrong, or the signal is odd
    Timeout,    // the completion signal never retired
    Rejected,   // invalid request / pre-submit failure / no libdrm
  };
  Status status = Status::Rejected;
  std::vector<std::uint8_t> output;
  // The completion signal as read back, for diagnosing a Timeout or a Fault.
  std::uint64_t signal_value = 0;
  std::vector<std::uint8_t> signal_raw;
  std::string detail;
};

struct ComputeSubmitRequest {
  // Kernel code, written verbatim into its BO at the entry VA.
  std::vector<std::uint8_t> text;
  // Bytes placed in the kernarg block AFTER the eight-byte output pointer.
  std::vector<std::uint8_t> kernarg_tail;
  std::uint32_t output_size = 64;
  std::uint32_t group[3] = { 1u, 1u, 1u };
  std::uint32_t grid[3] = { 1u, 1u, 1u };
  std::uint32_t timeout_ms = 3000;
  // Capture the whole signal allocation, not just the counter, so a
  // firmware-written timestamp block would be visible.
  bool dump_signal = false;
};

ComputeSubmitReport submit_compute(const ComputeSubmitRequest& request);

}  // namespace lgpu::drm
