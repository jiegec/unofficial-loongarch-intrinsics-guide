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
// the bytes of the kernarg block.  The kernarg's first eight bytes are ALWAYS
// the output BO's GPU VA, because that is where a compiled kernel's first user
// argument lands (`load.s.flat.w2 s[0:1], s[6:7], zero, 0x0`); the caller's own
// bytes follow at +8.
//
// Why this carries a post-mortem: the CPIPE user queue has no BO validation
// list, so a stream the firmware rejects - or a launch whose VAs are not the
// ones the plan published - does not return an error, it wedges the pipe and
// costs a platform power cycle (measured twice: "the CP latch needs platform
// power (S3/reboot)").  A wedge that leaves no record therefore costs a reboot
// and teaches nothing.  This function fills the WEDGE RECORD before the
// doorbell and prints it on any failure.
struct ComputeSubmitReport {
  enum class Status {
    Clean,      // doorbell rung, completion signal moved, output read back
    Fault,      // completed but the signal did not move as the ABI says
    Timeout,    // the completion signal never moved
    Rejected,   // invalid request / pre-submit failure / no libdrm
  };
  Status status = Status::Rejected;
  std::vector<std::uint8_t> output;
  // The completion signal as read back.  The device DECREMENTS it (HSA
  // semantics: the host arms the number of outstanding packets and waits for
  // zero), so arming 1 and seeing 0 is the clean case and arming 0 and seeing
  // 0xffffffffffffffff is the same event.  The value is reported either way, so
  // which direction the hardware moves it is measured rather than assumed.
  std::uint64_t signal_value = 0;
  std::vector<std::uint8_t> signal_raw;
  // Exactly what was submitted, filled before the doorbell so a timeout still
  // has it: the MCS stream, the 96-byte kernel descriptor, the kernarg block,
  // and the VA/geometry fields as one line of text.
  std::vector<std::uint8_t> wedge_stream;
  std::vector<std::uint8_t> wedge_kd;
  std::vector<std::uint8_t> wedge_kernarg;
  std::string wedge_fields;
  std::string detail;
};

struct ComputeSubmitRequest {
  // Kernel code, written verbatim into its BO at the entry VA.
  std::vector<std::uint8_t> text;
  // Bytes placed in the kernarg block AFTER the eight-byte output pointer: the
  // user-argument + implicit-argument region the vendor producer defines, the
  // implicit block at kernarg + user_args_size.  The caller owns that layout
  // and this layer only writes it.
  std::vector<std::uint8_t> kernarg_tail;
  // (byte offset, dword) pairs preloaded into the output BO before the
  // doorbell.  A carrier stages its input window here so the runner never has
  // to know a carrier's addressing.
  std::vector<std::pair<std::uint32_t, std::uint32_t>> output_preload;
  std::uint32_t output_size = 64;
  // Register counts declared in the kernel descriptor.  Not free fields: they
  // are the wave's SGPR/VGPR allocation (granulated in eights and fours), and a
  // program that uses a register outside its declared allocation is undefined.
  // The emitter reports its own high-water mark and the caller passes it here,
  // so the descriptor and the program cannot disagree.
  std::uint32_t num_sgprs = 32;
  std::uint32_t num_vgprs = 32;
  // Declared workgroup shared memory, in bytes; 0 for the flat carriers.
  std::uint32_t shared_segment_size = 0;
  std::uint32_t group[3] = { 1u, 1u, 1u };
  std::uint32_t grid[3] = { 1u, 1u, 1u };
  std::uint32_t timeout_ms = 3000;
  // Reuse one process-lifetime DRM file and user queue across every submission
  // made with this request set, instead of opening and tearing down a session
  // per submission.  The teardown path is where a few hundred create/destroy
  // cycles wedge the pipe, so the bulk (batch) entry point sets this; a
  // one-shot caller leaves it false and keeps full per-submission isolation.
  bool persistent_session = false;
  // Capture the whole signal allocation, not just the counter, so a
  // firmware-written timestamp block would be visible.
  bool dump_signal = false;
};

ComputeSubmitReport submit_compute(const ComputeSubmitRequest& request);

// Tear the process-lifetime compute session down in order: drain the ring, free
// the ONE queue the persistent session created, then close the DRM file.  Call
// it at the end of a batch; without it the queue is left to DRM file close,
// which is the path whose failure retains the VM.
void close_cached_session();

// The GPU virtual addresses the compute carrier uses.  Exported because the
// caller must assemble the same addresses into its programs and its kernarg
// block: the plan, the program and the BOs have to agree, and a disagreement
// is what wedges the pipe instead of returning an error.
struct ComputeVaLayout {
  std::uint64_t ring_base;   // 64-byte index block, then the ring
  std::uint64_t text;
  std::uint64_t kd;
  std::uint64_t kernarg;
  std::uint64_t output;
  std::uint64_t signal;
};
const ComputeVaLayout& compute_va_layout();

// Host-side preflight for the compute path: no device, no submission.
//
// The CPIPE user queue has no BO validation list, so a stream the firmware
// rejects does not return an error - it wedges the pipe and costs a platform
// power cycle.  The only defence available before the doorbell is to prove the
// bytes are the ones the vendor's own runtime produces.  This rebuilds the MCS
// stream and the 96-byte kernel descriptor for the two byte-exact vendor
// references and compares them byte for byte:
//
//   * the live 1x1x1 add.cl dispatch captured at AqlQueue::SubmitMCS
//     (analysis/lacm/lggpu-abi/MCS-ENCODING.md, "Ground truth"), and
//   * the same vendor emitters driven with the CTS geometry 2x2x4 workgroups of
//     1x4x2 invocations (analysis/lacm/mcs_ref_geom.c), whose value is that a
//     swapped or transposed dimension still produces the 1x1x1 bytes.
//
// It also checks that a maximal stream fits the ring slot the driver publishes.
// Returns true and fills `detail` with what was checked, or false with the
// first difference spelled out.  Callers must refuse to submit on false.
bool compute_selfcheck(std::string& detail);

}  // namespace lgpu::drm
