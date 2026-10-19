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
  // Observe mode: the carrier executes and reports the readback values
  // without value comparison (SIO file contents are not oracle-able).
  bool observe = false;
  // 1-ulp tolerant compare for the hardware-approximation families.
  bool approx1ulp = false;
  // GS family: bytes/words carry the GS program; fs_bytes carries the
  // companion fragment shader (solid export).
  std::vector<std::uint8_t> fs_bytes;
};

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

// Builds the generic PS program for one mnemonic and its case rows.
// Results must be 1-2 dwords and inputs must cover the operand template.
// VCC-output mnemonics keep only the primary destination dword. Returns
// nullopt with a readable error otherwise.
std::optional<EnvelopeProgram> build_ps(const std::string& mnemonic,
                                        const std::vector<EnvelopeCase>& rows,
                                        std::string& error);



// True for the control-prefix family (no numeric result).
bool is_control(const std::string& mnemonic);

// Control (execute-and-observe) carrier: control-prefix family.
std::optional<EnvelopeProgram> build_control(const std::string& mnemonic,
                                             const std::vector<EnvelopeCase>& rows,
                                             std::string& error);

// Environment-register sweep: executes getreg s8,<id> per entry; each read
// value is stored at 0x40 + 4*i in the argument BO.  Returns nullopt with
// error on assembler failure.
std::optional<EnvelopeProgram> build_getreg_sweep(
    const std::vector<std::uint32_t>& ids, std::string& error);

// Movindex window probe: s4=<base>, s5=<idx>, s6=0 with s20=<magic>
// preloaded; s_movindex_b32 s8,s4,s5,s6, result stored at 0x40.
std::optional<EnvelopeProgram> build_movidx_probe(
    std::uint32_t idx, std::uint32_t magic, std::string& error);
std::optional<EnvelopeProgram> build_movidx_base_probe(
    std::uint32_t base, std::uint32_t idx, std::uint32_t magic,
    std::string& error);

// Flat-address memory carrier: load_v_flat_*/store_v_flat_*/atomic_v_flat_*.
std::optional<EnvelopeProgram> build_flat(const std::string& mnemonic,
                                          const std::vector<EnvelopeCase>& rows,
                                          std::string& error);
std::optional<EnvelopeProgram> build_buff(const std::string& mnemonic,
                                          const std::vector<EnvelopeCase>& rows,
                                          std::string& error);

// Workgroup shared memory carrier: load_sm_*/store_sm_*.
std::optional<EnvelopeProgram> build_lds(const std::string& mnemonic,
                                         const std::vector<EnvelopeCase>& rows,
                                         std::string& error);

// Texture fetch carrier: load_v_tex with an explicitly staged T# tuple.
std::optional<EnvelopeProgram> build_tex_store(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    std::string& error);
std::optional<EnvelopeProgram> build_tex_atomic(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    std::string& error);
std::optional<EnvelopeProgram> build_samp(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    std::string& error);
std::optional<EnvelopeProgram> build_tex(const std::string& mnemonic,
                                         const std::vector<EnvelopeCase>& rows,
                                         std::string& error);

// Single entry point: routes one mnemonic to its carrier (generic, control,
// buff, flat, lds, tex/atex/stex, samp) by family prefix, in one place.
std::optional<EnvelopeProgram> build_carrier(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    std::string& error);

}  // namespace lgpu::envelope
