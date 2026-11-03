#pragma once

// PS-carrier envelope: materializes one mnemonic's data cases into a
// generic PS program.
// Imports the codec layer only: no DRM, no case semantics of its own.

#include "codec/operand.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace lgpu::envelope {

// One case row, mirroring case.toml [[case]] data.
struct EnvelopeCase {
  std::string id;
  std::vector<std::uint32_t> inputs;
  std::vector<std::uint32_t> expected;
  std::string fp_policy;
};

// Offsets assigned to a case's result dwords inside the argument BO.
struct BuiltCase {
  std::string id;
  std::vector<std::uint32_t> offsets;
  std::vector<std::uint32_t> expected;
};

struct EnvelopeProgram {
  std::vector<std::uint8_t> bytes;        // 16-byte aligned shader body
  std::vector<isa::EncodedInst> words;    // encoded instructions incl. nop pad
  std::vector<BuiltCase> cases;           // accepted cases + result offsets
  std::string listing;                    // readable asm listing
  // (byte offset, dword) pairs to preload into the argument BO before the
  // submission; memory carriers declare their own load-window data here so
  // the runner can stage them without knowing carrier addressing.
  std::vector<std::pair<std::uint32_t, std::uint32_t>> arg_preloads;
  // Transport hints set by the carrier (the runner never re-derives them
  // from the mnemonic): probe = memory carrier needing the probe roots;
  // control = sentinel observation semantics (survived vs terminated).
  bool probe = false;
  bool control = false;
  // Which outcome the control carrier's sentinel pair must show.  The outcome
  // is carried by the row's own description, not by the hardware: a control row
  // that accepts either outcome can never fail, so none of the 20 rows in this
  // family asserts anything.  0 = the row settles neither (the run records the
  // outcome and claims nothing), 1 = survived (the instruction leaves the
  // carrier's control flow alone, so the post store executes), 2 = terminated
  // (it transfers control somewhere that is not the next instruction, so the
  // post store does not execute).
  int control_expect = 0;
  // Observe mode: the carrier executes and reports the readback values
  // without value comparison (SIO file contents are not oracle-able).
  bool observe = false;
  // Why observe is set, in the carrier's own words.  The runner prints it so
  // the results matrix carries the reason beside the row, instead of leaving a
  // reader to infer one from the mnemonic.
  const char* observe_reason = "";
  // 1-ulp tolerant compare for the hardware-approximation families.
  bool approx1ulp = false;
  // Highest SGPR index + 1 and VGPR index + 1 the program touches, derived
  // from the emitted operands.  The kernel descriptor declares the wave's
  // register allocation from these; a program that uses a register outside its
  // own declaration is undefined, so the compute path passes them through
  // rather than declaring a fixed window.
  std::uint32_t sgpr_high_water = 0;
  std::uint32_t vgpr_high_water = 0;
  // GS family: bytes/words carry the GS program; fs_bytes carries the
  // companion fragment shader (solid export).
  std::vector<std::uint8_t> fs_bytes;
  // Declared workgroup shared memory for the compute launch, in bytes.  Only
  // the mask probes that stage cells in LDS set it; everything else leaves it
  // zero so the kernel descriptor keeps declaring no shared memory.
  std::uint32_t shared_bytes = 0;
};

// Declared workgroup shared memory for a launch that executes an LDS
// instruction.  The size belongs to the launch, not to the row: it only has to
// cover the cells the carrier stages, and 4 KiB covers every case shape (one
// cell per row at 0x20 + 4*width*chunk, chunk 64 and width at most 4: under 1.1
// KiB).  It is also a whole number of resource-word granules - shape_rsrc0
// folds the size in as ((bytes*8 + 0x3f8) & 0x1ffc00), one unit per 1024 bytes
// - so a sub-1024 declaration leaves granulated_shm_size at zero while .kd +28
// names the window, and the two fields disagree about whether the launch has
// shared memory at all.
inline constexpr std::uint32_t kLdsWindowBytes = 4096;

// True when the program's listing carries a shared-memory instruction.  The
// builder (which must declare a window) and the runner (which refuses a
// windowless launch) ask that one question through this predicate, so they
// cannot disagree about which programs need a window.
bool uses_shared_memory(const EnvelopeProgram& program);

// True when the mnemonic has a materializable generic PS suite.
bool generic_available(const std::string& mnemonic);

// The operand-kind form produced by the generic emitter.
std::string generic_form(const std::string& mnemonic);

// Benchmark template: K copies of the mnemonic inside one PS program.
// chained=true binds every source to the previous destination (dependency
// chain, latency template); chained=false rotates sources through an 8-way
// register window (independent streams, throughput template).  Immediates
// are 0, predicate p0, VCC preloaded to 0.  Uses the same materializability
// rules as build_ps.
std::optional<EnvelopeProgram> build_bench(const std::string& mnemonic,
                                           unsigned copies, bool chained,
                                           std::string& error);

// The compute-pipe counterpart: the same instruction template emitted through
// the generic compute carrier (flat result store, CarrierEnv::compute), so a
// benchmark and the validated CS matrix share one code path.  The timestamps
// come from the firmware block in the signal allocation, which the submit layer
// already writes; see the CS microbench section of the README.
std::optional<EnvelopeProgram> build_cs_bench(const std::string& mnemonic,
                                              unsigned copies, bool chained,
                                              unsigned lanes,
                                              std::string& error);

// A per-mnemonic note for the bench CSV, for rows whose measurement is not what
// the column name implies (a store's lat column is a store+load round trip).
const char* bench_note(const std::string& mnemonic);

// Builds the generic PS program for one mnemonic and its case rows.
// Results must be 1-2 dwords and inputs must cover the operand template.
// VCC-output mnemonics keep only the primary destination dword. Returns
// nullopt with a readable error otherwise.
std::optional<EnvelopeProgram> build_ps(const std::string& mnemonic,
                                        const std::vector<EnvelopeCase>& rows,
                                        std::string& error);

// The two launch environments the generic emitter can target.  They share
// everything except where the result-store buffer descriptor's base comes
// from: Graphics assembles the argument BO's fixed VA, Compute reads the
// output BO's VA out of the kernarg block the hardware points s[6:7] at.
enum class LaunchEnvironment { Graphics, Compute };

// How many lanes each launch engine executes.  The graphics fragment launch
// runs a 4x4 region; a compute dispatch runs exactly the workgroup it is
// handed, so the compute side has no constant and must be told (cs_group() in
// main.cpp is the one place that decides it).  The per-lane atomics are the
// consumers: see atomic_final_n in envelope.cpp.
inline constexpr unsigned kGraphicsLanes = 16;

// Builds the same generic program for a compute (CPIPE) dispatch.  On the
// compute stage there is no constant root and no hydrated user data, so the
// result buffer is reached by the absolute VA in carrier_map.hpp
// (carrier::kCsArgVa), which drm::submit_compute maps the output BO at - the
// two sides share that constant rather than deriving the address at runtime.
// The launch contract is still loongvk's: the .kd enables en_sgpr_kernarg_ptr,
// so s[6:7] holds the kernarg VA, and the carrier reads it once as launch-state
// evidence (the host writes the output BO's VA as the first user argument
// there).  Using a literal instead of deriving the address from s[6:7] is a
// deliberate deviation; see README, "Launch and memory path vs loongvk".
//
// `lanes` is the number of work-items the dispatch will execute (the product
// of the group's dimensions).  It is a required argument rather than a default
// so that the expected values and the launch cannot disagree: a per-lane
// atomic's expectation is a function of this number.
std::optional<EnvelopeProgram> build_cs(const std::string& mnemonic,
                                        const std::vector<EnvelopeCase>& rows,
                                        std::string& error, unsigned lanes);



// True for the control-prefix family (no numeric result).
bool is_control(const std::string& mnemonic);

// Environment-register sweep: executes getreg s8,<id> per entry; each read
// value is stored at 0x40 + 4*i in the argument BO.  Returns nullopt with
// error on assembler failure.
std::optional<EnvelopeProgram> build_getreg_sweep(
    const std::vector<std::uint32_t>& ids, std::string& error);

// Selector sweep: executes v_mov.b32 p0,v8,<raw selector byte> per entry and
// stores the result at 0x40 + 4*i.  Probe-only: the selector is a literal
// byte, not a name, so it can reach encodings the assembler will not spell.
std::optional<EnvelopeProgram> build_selector_sweep(
    const std::vector<std::uint32_t>& selectors, std::string& error);

// The same sweep through the SOP format (s_mov.b32 s8,<byte>).  GJ8 and GJ11
// share the GeneralSource8 encoding, so running both shows whether a byte's
// value is a property of the selector space or of one instruction's decoder.
std::optional<EnvelopeProgram> build_selector_sweep_sop(
    const std::vector<std::uint32_t>& selectors, std::string& error);

// High-bit sweep of the 11-bit general source: for one 8-bit base, emit every
// combination of the three high bits (0x100 vector domain, 0x200 clear sign,
// 0x400 flip sign).  v8 is preloaded with -1.0f and the destination is v9.
std::optional<EnvelopeProgram> build_selector_hi_sweep(std::uint32_t base,
                                                       std::string& error);

// Movindex window probe: s4=<base>, s5=<idx>, s6=0 with s20=<magic>
// preloaded; s_movindex_b32 s8,s4,s5,s6, result stored at 0x40.
std::optional<EnvelopeProgram> build_movidx_probe(
    std::uint32_t idx, std::uint32_t magic, std::string& error);
std::optional<EnvelopeProgram> build_movidx_base_probe(
    std::uint32_t base, std::uint32_t idx, std::uint32_t magic,
    std::string& error);

// Single entry point: routes one mnemonic to its carrier family in one place.
// `kind` selects the launch environment, which changes only where the result
// buffer lives and how it is reached - never which family owns a mnemonic.
// `lanes` is the launch's executed lane count (see kGraphicsLanes / build_cs).
std::optional<EnvelopeProgram> build_carrier(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    LaunchEnvironment kind, std::string& error,
    unsigned lanes = kGraphicsLanes);

// The compute launch-state self-test: reads the kernarg pointer's first user
// argument and the group/local id registers and stores all eight dwords at
// output offsets 0..28.  Returns nullopt with a readable error on assembler
// failure.
std::optional<EnvelopeProgram> build_cs_probe(std::string& error,
                                             unsigned lanes);

// U-2 / P0-R10 raw-shader sequence probe (LCO X-1): write a magic value into
// the saved lane-mask predicate p1, optionally disturb the lane mask p0,
// then read p1/p0/p2/p3 back and store them at output offsets 0/4/8/12.
// Variants: 0 = plain write/read, 1 = s_wqm_b32 p0,p0, 2 = narrow p0 with
// s_and_b32, 3 = barrier, 4 = write p2/p3 as well.  p1 is expected to read the
// magic back in every variant if it is ordinary per-wave storage.
std::optional<EnvelopeProgram> build_cs_mask_probe(unsigned variant,
                                                   std::string& error,
                                                   unsigned lanes);

// E-1: the same carrier, but every instruction under test carries an explicit
// PJ2 selector.  Variants 20-23 write a vector destination under PJ2=k with
// known p0/p1 contents and store the result per lane; 24 reads the selected
// mask back; 25/26 isolate a vector store's gating; 27/28 branch on p1.
// 40/41 extend the gating question to the vop-imm layout, 42-44 to the shared
// memory layout (store, load, mask survival), 45/46 ask whether czb reads a
// mask register, 47-49 whether s_wqm_b32 accepts mask registers as source and
// destination, 50 whether an instruction may select and write the same mask,
// and 51/52 whether the pN file aliases the sGPR window.
std::optional<EnvelopeProgram> build_cs_pj2_probe(unsigned variant,
                                                  std::string& error,
                                                  unsigned lanes);

// U-4: is "check" legal inside a mask-narrowed region?  Two arms, each narrowing
// from the launch's own mask, each storing a constant to its own cell and
// loading it back, and each recording which lanes ran it.  Output: 0x40/0x44 =
// arm A's stored value / readback, 0x48/0x4c = arm B's, 0x50 = the restored p0,
// 0x200 + 4*lane = arm A's lane record, 0x300 + 4*lane = arm B's (pre-filled
// with 0xdeadbeef).  Variants: 0 = no check between the store and the load
// (control), 1 = check k|vm|vs|sm inside arm A, 2 = check vm, 3 = check k,
// 4 = a check in both arms, 5 = one convergent check before the arms (control
// for 1), 6 = arm B (odd lanes) first, 7 = like 1 with the full launch mask
// (separates "diverged" from "check present").
std::optional<EnvelopeProgram> build_cs_divergent_check_probe(unsigned variant,
                                                              std::string& error,
                                                              unsigned lanes);

// U-6: what do the matrix loads read?  Fills 64 LDS cells with
// 0xab000000 | index, then reads one dword per lane and records it at
// 0x100 + 4*lane (0x200/0x300/0x400 hold components 1..3 for the x2/x4 forms).
// Variants: 0 = load_sm_b32 control, 1 = matrix_x1 at a 4-byte stride,
// 2 = matrix_x2, 3 = matrix_x4, 4 = matrix_x1 at a 16-byte stride, 5 =
// matrix_x1 with the same address in every lane.
std::optional<EnvelopeProgram> build_cs_matrix_probe(unsigned variant,
                                                     std::string& error,
                                                     unsigned lanes);

// U-7: does a lane mask survive N unrelated operations?  Writes the magic into
// p1, runs N disturbances, then reads p0..p3 back to 0x10/0x14/0x18/0x1c.
// Variant is 100*class + N; class 1 = vector ALU, 2 = flat vector store,
// 3 = LDS store+load, 4 = check, 5 = barrier, 6 = a compare writing p2,
// 7 = the mixed chain of all six per round.
std::optional<EnvelopeProgram> build_cs_mask_lifetime_probe(unsigned variant,
                                                            std::string& error,
                                                            unsigned lanes);

// The same mask-register sequence through the graphics PS carrier (4x4 = 16
// lanes, fragment stage).  Same write/read mechanism and output offsets; the
// point is whether the fragment stage's mask window behaves like the compute
// pipe's, since that is where WQM and helper lanes live.  Variants 10-12 run
// s_wqm_b32 through the mask file, 13 asks whether a narrowed p0 really gates
// the stage's vector writes, 14 whether the selected mask survives a vector
// store, and 15 is the whole-quad prologue/epilogue round trip.
std::optional<EnvelopeProgram> build_ps_mask_probe(unsigned variant,
                                                   std::string& error);

}  // namespace lgpu::envelope
