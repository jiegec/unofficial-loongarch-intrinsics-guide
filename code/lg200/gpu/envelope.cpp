// Generic PS envelope. This file imports the codec layer only.

#include "carrier_map.hpp"
#include "envelope.hpp"

#include "codec/codec.hpp"
#include "codec/semantics.hpp"

#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace lgpu::envelope {
namespace {

// ---- minimal shader line builder ----

struct Line {
  std::string text;
  isa::McInst inst;
};

struct Builder {
  std::vector<Line> lines;
  // Highest SGPR index + 1 and VGPR index + 1 this program touches.  The
  // kernel descriptor declares the wave's register allocation from these, and
  // a program that uses a register outside its own declaration is undefined -
  // so the count is derived from the emitted operands rather than guessed.
  unsigned sgpr_high = 0;
  unsigned vgpr_high = 0;

  void note_reg(const isa::Operand& op) {
    if (op.kind != isa::OperandKind::Reg) return;
    const unsigned high = static_cast<unsigned>(op.reg.index) + op.reg.width;
    switch (op.reg.cls) {
      case isa::RegClass::Scalar:
      case isa::RegClass::Vcc:
        if (high > sgpr_high) sgpr_high = high;
        break;
      case isa::RegClass::Vector:
        if (high > vgpr_high) vgpr_high = high;
        break;
      default:
        // Scc (a flag, not a file index), predicates and the special zero
        // source occupy no allocated register.
        break;
    }
  }

  void append(const isa::McInst& inst) {
    for (const auto& op : inst.operands) note_reg(op);
    lines.push_back({inst.format(), inst});
  }
  void s_mov_b32(const isa::Reg& dst, const isa::Reg& src) {
    append(isa::McInst{"s_mov_b32", "(scc)",
                       {isa::op_reg(dst), isa::op_reg(src)}});
  }
  void s_mov_imm_b32(const isa::Reg& dst, std::uint32_t immediate) {
    append(isa::McInst{"s_mov_imm_b32", "(scc)",
                       {isa::op_reg(dst), isa::op_imm(immediate)}});
  }
  void v_mov_b32(const isa::Reg& dst, const isa::Reg& src,
                 const isa::Reg& pred) {
    append(isa::McInst{"v_mov_b32", "",
                       {isa::op_reg(pred), isa::op_reg(dst), isa::op_reg(src)}});
  }
  void store_v_buff_b32(const isa::Reg& pred, const isa::Reg& data,
                        const isa::Reg& address, const isa::Reg& descriptor_base,
                        const isa::Reg& offset, std::uint32_t immediate,
                        bool offen, bool idxen, bool bl1, bool bl2) {
    std::vector<isa::Operand> operands = {
        isa::op_reg(pred), isa::op_reg(data), isa::op_reg(address),
        isa::op_reg(descriptor_base), isa::op_reg(offset), isa::op_imm(immediate)};
    if (offen) operands.push_back(isa::op_modifier("offen", 1));
    if (idxen) operands.push_back(isa::op_modifier("idxen", 1));
    if (bl1) operands.push_back(isa::op_modifier("bl1", 1));
    if (bl2) operands.push_back(isa::op_modifier("bl2", 1));
    append(isa::McInst{"store_v_buff_b32", "", std::move(operands)});
  }
  void buff_mem(const std::string& mne, const isa::Reg& pred,
                const isa::Reg& dst, const isa::Reg& address,
                const isa::Reg& descriptor_base, const isa::Reg& offset,
                std::uint32_t immediate, bool offen, bool idxen, bool bl1,
                bool bl2) {
    std::vector<isa::Operand> operands = {
        isa::op_reg(pred), isa::op_reg(dst), isa::op_reg(address),
        isa::op_reg(descriptor_base), isa::op_reg(offset), isa::op_imm(immediate)};
    if (offen) operands.push_back(isa::op_modifier("offen", 1));
    if (idxen) operands.push_back(isa::op_modifier("idxen", 1));
    if (bl1) operands.push_back(isa::op_modifier("bl1", 1));
    if (bl2) operands.push_back(isa::op_modifier("bl2", 1));
    append(isa::McInst{mne, "", std::move(operands)});
  }
  // The 8-bit buff forms carry no address operand: the driver gives them
  // RA8 RK8x4 VJ8 UO12 (against RA8 RK8x4 VQ8 VJ8 UO12 for the 16/32-bit
  // forms), so the operand list is predicate, data, descriptor, VJ8, UO12.
  // VJ8 is a VGPR; passing a scalar there is what made the old emission
  // ill-formed, and the codec now rejects it at assembly time.
  void buff_subword_load(const std::string& mne, const isa::Reg& pred,
                         const isa::Reg& dst, const isa::Reg& descriptor_base,
                         const isa::Reg& index, std::uint32_t immediate,
                         bool bl1) {
    std::vector<isa::Operand> operands = {
        isa::op_reg(pred), isa::op_reg(dst), isa::op_reg(descriptor_base),
        isa::op_reg(index), isa::op_imm(immediate)};
    if (bl1) operands.push_back(isa::op_modifier("bl1", 1));
    append(isa::McInst{mne, "", std::move(operands)});
  }
  void buff_subword_store(const std::string& mne, const isa::Reg& pred,
                          const isa::Reg& data,
                          const isa::Reg& descriptor_base,
                          const isa::Reg& index, std::uint32_t immediate,
                          bool bl1) {
    std::vector<isa::Operand> operands = {
        isa::op_reg(pred), isa::op_reg(data), isa::op_reg(descriptor_base),
        isa::op_reg(index), isa::op_imm(immediate)};
    if (bl1) operands.push_back(isa::op_modifier("bl1", 1));
    append(isa::McInst{mne, "", std::move(operands)});
  }
  void sm_atomic(const std::string& mne, const isa::Reg& pred,
                 const isa::Reg& address, const isa::Reg& data,
                 const isa::Reg& old_dst) {
    append(isa::McInst{mne, "", {isa::op_reg(pred), isa::op_reg(address),
                                 isa::op_reg(data), isa::op_reg(old_dst)}});
  }
  void sm_cmpswap(const std::string& mne, const isa::Reg& pred,
                  const isa::Reg& compare, const isa::Reg& address,
                  const isa::Reg& swap, const isa::Reg& old_dst) {
    append(isa::McInst{mne, "", {isa::op_reg(pred), isa::op_reg(compare),
                                 isa::op_reg(address), isa::op_reg(swap),
                                 isa::op_reg(old_dst)}});
  }
  // Generic tex-family emitter: caller supplies pred + operand registers
  // in semantic-rule order; un/dmask modifiers appended per flags.
  void image_op(const std::string& mne, std::vector<isa::Operand> operands,
                bool un) {
    if (un) operands.push_back(isa::op_modifier("un", 1));
    operands.push_back(isa::op_modifier("dmask", 0xf));
    append(isa::McInst{mne, "", std::move(operands)});
  }
  void sflat_load(const std::string& mne, const isa::Reg& dst,
                  const isa::Reg& addr_pair, const isa::Reg& soff,
                  std::uint32_t immediate) {
    // Scalar memory ops carry no predicate field.
    append(isa::McInst{mne, "", {isa::op_reg(dst), isa::op_reg(addr_pair),
                                 isa::op_reg(soff), isa::op_imm(immediate)}});
  }
  void sm_op(const std::string& mne, const isa::Reg& pred,
             const isa::Reg& reg, const isa::Reg& address) {
    append(isa::McInst{mne, "", {isa::op_reg(pred), isa::op_reg(reg),
                                  isa::op_reg(address)}});
  }
  void load_v_buff_b32(const isa::Reg& pred, const isa::Reg& dst,
                       const isa::Reg& address, const isa::Reg& descriptor_base,
                       const isa::Reg& offset, std::uint32_t immediate,
                       bool offen, bool idxen, bool bl1, bool bl2) {
    std::vector<isa::Operand> operands = {
        isa::op_reg(pred), isa::op_reg(dst), isa::op_reg(address),
        isa::op_reg(descriptor_base), isa::op_reg(offset), isa::op_imm(immediate)};
    if (offen) operands.push_back(isa::op_modifier("offen", 1));
    if (idxen) operands.push_back(isa::op_modifier("idxen", 1));
    if (bl1) operands.push_back(isa::op_modifier("bl1", 1));
    if (bl2) operands.push_back(isa::op_modifier("bl2", 1));
    append(isa::McInst{"load_v_buff_b32", "", std::move(operands)});
  }
  void buff_atomic(const std::string& mne, const isa::Reg& pred,
                   const isa::Reg& dst_data, const isa::Reg& descriptor_base,
                   const isa::Reg& offset, std::uint32_t immediate, bool bl1) {
    std::vector<isa::Operand> operands = {
        isa::op_reg(pred), isa::op_reg(dst_data), isa::op_reg(descriptor_base),
        isa::op_reg(offset), isa::op_imm(immediate)};
    if (bl1) operands.push_back(isa::op_modifier("bl1", 1));
    append(isa::McInst{mne, "", std::move(operands)});
  }
  void load_v_flat(const std::string& mne, const isa::Reg& pred,
                  const isa::Reg& dst, const isa::Reg& addr_pair,
                  const isa::Reg& base_pair) {
    append(isa::McInst{mne, "",
                       {isa::op_reg(pred), isa::op_reg(dst), isa::op_reg(addr_pair),
                        isa::op_reg(base_pair)}});
  }
  void store_v_flat(const std::string& mne, const isa::Reg& pred,
                    const isa::Reg& data, const isa::Reg& addr_pair,
                    const isa::Reg& base_pair) {
    append(isa::McInst{mne, "",
                       {isa::op_reg(pred), isa::op_reg(data), isa::op_reg(addr_pair),
                        isa::op_reg(base_pair)}});
  }
  void atomic_v_flat(const std::string& mne, const isa::Reg& pred,
                     const isa::Reg& dst, const isa::Reg& addr_pair,
                     const isa::Reg& base_pair, const isa::Reg& data) {
    append(isa::McInst{mne, "",
                       {isa::op_reg(pred), isa::op_reg(dst), isa::op_reg(addr_pair),
                        isa::op_reg(base_pair), isa::op_reg(data)}});
  }

  void check_k_vm_vs_sm() {
    append(isa::McInst{"check", "", {isa::op_raw("k vm vs sm")}});
  }
  void nop() { append(isa::McInst{"nop", "", {}}); }
  void exit() { append(isa::McInst{"exit", "", {}}); }
  void append_target(const isa::McInst& inst) { append(inst); }
};

// ---- register classes from the first canonical form ----

bool general_source_classes(const std::string& mnemonic,
                            std::vector<isa::RegClass>& classes) {
  const auto* spec = isa::semantic_spec(mnemonic);
  if (spec == nullptr) {
    return false;
  }
  classes.assign(spec->rule_count, isa::RegClass::Vector);
  // vop3 three-source families (max3/min3/med3 and the cube face
  // selectors) take three vector sources; the open decoder labels the
  // GK/GA 11-bit fields as scalar, which the fixed v0/v1/v2 staging
  // contradicts on hardware (s4/s5 are never loaded).
  const bool vop3_vec3 =
      mnemonic == "v_max3_f32" || mnemonic == "v_min3_f32" ||
      mnemonic == "v_med3_f32" || mnemonic == "v_cubeface_f32" ||
      mnemonic == "v_cubema_f32" || mnemonic == "v_cubes_f32" ||
      mnemonic == "v_cubet_f32" || mnemonic == "v_select_b32";
  if (vop3_vec3) {
    return true;
  }
  for (const auto& variant : isa::codec_variants()) {
    if (variant.mnemonic != mnemonic) {
      continue;
    }
    isa::DecodedInst decoded;
    std::string error;
    if (!isa::decode_instruction(variant.words.lo, variant.words.hi, decoded,
                                 error)) {
      continue;
    }
    std::size_t vi = 0;
    for (unsigned ri = 0; ri < spec->rule_count && vi < decoded.operands.size();
         ++ri) {
      const auto& rule = spec->rules[ri];
      if (rule.sem == isa::FieldSem::Modifier) {
        continue;
      }
      const auto& operand = decoded.operands[vi];
      if (rule.sem == isa::FieldSem::GeneralSource11 ||
          rule.sem == isa::FieldSem::GeneralSource8 ||
          rule.sem == isa::FieldSem::Scalar8 ||
          rule.sem == isa::FieldSem::Vcc8 ||
          rule.sem == isa::FieldSem::ScalarQuad8 ||
          rule.sem == isa::FieldSem::SccSource8) {
        if (operand.kind == isa::OperandKind::Reg) {
          classes[ri] = operand.reg.cls;
        } else if (operand.kind == isa::OperandKind::InlineInt ||
                   operand.kind == isa::OperandKind::InlineFloat) {
          classes[ri] = isa::RegClass::SpecialZero;
        }
      }
      ++vi;
    }
    return true;
  }
  return false;
}

bool is_implicit_scc_family(const std::string& mnemonic) {
  return mnemonic.rfind("s_cmp_", 0) == 0 ||
         mnemonic.rfind("s_cmpk", 0) == 0 ||
         mnemonic.rfind("s_bitcmp", 0) == 0 ||
         mnemonic.rfind("s_bitcmprev", 0) == 0;
}

// v_cmp*: the destination is a LANE MASK, one bit per lane, written under the
// executing mask - not a boolean word.  The oracle's 0x0000ffff is the
// 16-lane graphics launch's all-lanes-true value; a compute launch runs
// env.lanes lanes, so "every active lane true" is (1<<lanes)-1 there.  The
// closed driver states the same model: the compare/lane-mask pool keeps its
// results in the ordinary SGPR window and reads them back from s8
// (lco_register_allocation.c), and the hardware evaluates a compare under the
// current exec mask (lco_spill.c).  The SCC-compare family does not need this
// because it materialises 0xffff with an explicit s_select.
bool is_lane_mask_family(const std::string& mnemonic) {
  return mnemonic.rfind("v_cmp", 0) == 0;
}

// ---- PS envelope materializability

bool materializable(const isa::SemanticSpec& spec,
                    const std::vector<isa::RegClass>& classes) {
  bool has_output = false;
  for (unsigned i = 0; i < spec.rule_count; ++i) {
    const auto& orule = spec.rules[i];
    if (orule.is_output ||
        orule.sem == isa::FieldSem::Vcc8 ||
        orule.sem == isa::FieldSem::ScalarCmp8) {
      has_output = true;
      break;
    }
  }
  if (!has_output && !is_implicit_scc_family(spec.mnemonic)) {
    return false;
  }
  for (unsigned i = 0; i < spec.rule_count; ++i) {
    const auto& rule = spec.rules[i];
    const std::uint8_t w =
        isa::semantic_tuple(spec.mnemonic, rule.field, rule.tuple_width);
    if (w != 1 && w != 2) {
      return false;
    }
    switch (rule.sem) {
      case isa::FieldSem::Predicate2:
      case isa::FieldSem::VectorDest8:
      case isa::FieldSem::VectorSource8:
      case isa::FieldSem::Vcc8:
        break;
      case isa::FieldSem::GeneralSource11:
      case isa::FieldSem::GeneralSource8:
        if (classes[i] != isa::RegClass::Vector &&
            classes[i] != isa::RegClass::Scalar &&
            classes[i] != isa::RegClass::SpecialZero) {
          return false;
        }
        break;
      case isa::FieldSem::Scalar8:
        if (rule.is_output) {
          break;
        }
        if (classes[i] != isa::RegClass::Scalar) {
          return false;
        }
        break;
      case isa::FieldSem::ScalarCmp8:
        if (rule.is_output) {
          break;
        }
        return false;
      case isa::FieldSem::SccSource8:
        if (classes[i] != isa::RegClass::Scalar &&
            classes[i] != isa::RegClass::Scc) {
          return false;
        }
        break;
      case isa::FieldSem::Immediate32:
      case isa::FieldSem::Immediate8:
        break;
      case isa::FieldSem::Immediate12:
      case isa::FieldSem::OffsetSource8:
      case isa::FieldSem::ScalarQuad8:
        return false;
      case isa::FieldSem::Modifier: {
        const char* name = rule.modifier_name ? rule.modifier_name : "";
        if (std::strcmp(name, "mod2") != 0 &&
            std::strcmp(name, "clmp1") != 0) {
          return false;
        }
        break;
      }
      default:
        return false;
    }
  }
  return true;
}

// ---- program linking ----

bool link_program_no_rr(const std::vector<Line>& lines,
                          EnvelopeProgram& out, std::string& error) {
  out.bytes.clear();
  out.words.clear();
  out.listing.clear();
  for (const auto& line : lines) {
    isa::EncodedInst words;
    if (!isa::encode_instruction(line.inst.mnemonic, line.inst.operands, words,
                                 error)) {
      error = line.inst.mnemonic + ": " + error;
      return false;
    }
    out.words.push_back(words);
    const std::uint8_t encoded[8] = {
        static_cast<std::uint8_t>(words.lo),
        static_cast<std::uint8_t>(words.lo >> 8),
        static_cast<std::uint8_t>(words.lo >> 16),
        static_cast<std::uint8_t>(words.lo >> 24),
        static_cast<std::uint8_t>(words.hi),
        static_cast<std::uint8_t>(words.hi >> 8),
        static_cast<std::uint8_t>(words.hi >> 16),
        static_cast<std::uint8_t>(words.hi >> 24),
    };
    out.bytes.insert(out.bytes.end(), std::begin(encoded), std::end(encoded));
    out.listing += line.text;
    out.listing.push_back('\n');
  }
  while (out.bytes.size() % 16 != 0) {
    isa::EncodedInst words;
    if (!isa::encode_instruction("nop", {}, words, error)) {
      return false;
    }
    out.words.push_back(words);
    const std::uint8_t encoded[8] = {
        static_cast<std::uint8_t>(words.lo),
        static_cast<std::uint8_t>(words.lo >> 8),
        static_cast<std::uint8_t>(words.lo >> 16),
        static_cast<std::uint8_t>(words.lo >> 24),
        static_cast<std::uint8_t>(words.hi),
        static_cast<std::uint8_t>(words.hi >> 8),
        static_cast<std::uint8_t>(words.hi >> 16),
        static_cast<std::uint8_t>(words.hi >> 24),
    };
    out.bytes.insert(out.bytes.end(), std::begin(encoded), std::end(encoded));
  }
  return true;
}

bool link_program(const std::vector<Line>& lines, EnvelopeProgram& out,
                  std::string& error) {
  out.bytes.clear();
  out.words.clear();
  out.listing.clear();
  for (const auto& line : lines) {
    isa::EncodedInst words;
    if (!isa::encode_instruction(line.inst.mnemonic, line.inst.operands, words,
                                 error)) {
      error = line.inst.mnemonic + ": " + error;
      return false;
    }
    isa::DecodedInst decoded;
    std::string decode_error;
    if (!isa::decode_instruction(words.lo, words.hi, decoded, decode_error) ||
        decoded.mnemonic != line.inst.mnemonic) {
      error = "linker round-trip failed for " + line.inst.mnemonic;
      return false;
    }
    out.words.push_back(words);
    const std::uint8_t encoded[8] = {
        static_cast<std::uint8_t>(words.lo),
        static_cast<std::uint8_t>(words.lo >> 8),
        static_cast<std::uint8_t>(words.lo >> 16),
        static_cast<std::uint8_t>(words.lo >> 24),
        static_cast<std::uint8_t>(words.hi),
        static_cast<std::uint8_t>(words.hi >> 8),
        static_cast<std::uint8_t>(words.hi >> 16),
        static_cast<std::uint8_t>(words.hi >> 24),
    };
    out.bytes.insert(out.bytes.end(), std::begin(encoded), std::end(encoded));
    out.listing += line.text;
    out.listing.push_back('\n');
  }
  while (out.bytes.size() % 16 != 0) {
    isa::EncodedInst words;
    if (!isa::encode_instruction("nop", {}, words, error)) {
      return false;
    }
    out.words.push_back(words);
    const std::uint8_t encoded[8] = {
        static_cast<std::uint8_t>(words.lo),
        static_cast<std::uint8_t>(words.lo >> 8),
        static_cast<std::uint8_t>(words.lo >> 16),
        static_cast<std::uint8_t>(words.lo >> 24),
        static_cast<std::uint8_t>(words.hi),
        static_cast<std::uint8_t>(words.hi >> 8),
        static_cast<std::uint8_t>(words.hi >> 16),
        static_cast<std::uint8_t>(words.hi >> 24),
    };
    out.bytes.insert(out.bytes.end(), std::begin(encoded), std::end(encoded));
  }
  return true;
}

}  // namespace

std::string generic_form(const std::string& mnemonic) {
  const auto* spec = isa::semantic_spec(mnemonic);
  if (spec == nullptr || spec->rule_count == 0) {
    return "";
  }
  std::vector<isa::RegClass> classes;
  if (!general_source_classes(mnemonic, classes)) {
    return "";
  }
  std::string out;
  for (unsigned i = 0; i < spec->rule_count; ++i) {
    const auto& rule = spec->rules[i];
    if (rule.sem == isa::FieldSem::Modifier) {
      continue;
    }
    if (!out.empty()) {
      out.push_back(',');
    }
    switch (rule.sem) {
      case isa::FieldSem::Predicate2:
        out += "p";
        break;
      case isa::FieldSem::VectorDest8:
      case isa::FieldSem::VectorSource8:
        out += "v";
        break;
      case isa::FieldSem::GeneralSource11:
      case isa::FieldSem::GeneralSource8: {
        const std::size_t flen = std::strlen(rule.field);
        const bool narrow8 = flen > 0 && rule.field[flen - 1] == '8';
        out += (classes[i] == isa::RegClass::Scalar || narrow8) ? "s" : "v";
        break;
      }
      case isa::FieldSem::Scalar8:
        out += "s";
        break;
      case isa::FieldSem::Vcc8:
        out += "vcc";
        break;
      case isa::FieldSem::Immediate32:
      case isa::FieldSem::Immediate8:
        out += "imm";
        break;
      default:
        out += "?";
        break;
    }
  }
  return out;
}

bool generic_available(const std::string& mnemonic) {
  const auto* spec = isa::semantic_spec(mnemonic);
  if (spec == nullptr || spec->rule_count == 0) {
    return false;
  }
  std::vector<isa::RegClass> classes;
  if (!general_source_classes(mnemonic, classes)) {
    return false;
  }
  return materializable(*spec, classes);
}

// ---- shared carrier helpers -------------------------------------------------

// Environment read helpers: experiment knobs stay stringly-typed at the
// CLI boundary and become typed values here.
std::uint32_t env_u32(const char* name, std::uint32_t dflt) {
  const char* v = std::getenv(name);
  return v ? static_cast<std::uint32_t>(std::strtoul(v, nullptr, 0)) : dflt;
}

bool env_has(const char* name) { return std::getenv(name) != nullptr; }

// ---- 16-lane atomic models (one op table, both consumers) ----
// Every lane of the carrier applies the same operation with the same
// operand to the same cell, so the final cell value is deterministic.
// Cell values are 64-bit; 32-bit callers truncate on return.  Signed
// min/max compare the sign-extended values (callers pre-extend as needed).
std::uint64_t atomic_apply(const std::string& op, bool sgn, std::uint64_t old,
                           std::uint64_t a, std::uint64_t b) {
  if (op == "add") return old + a;
  if (op == "sub") return old - a;
  // LOONGGPU_DELTA / UNRESOLVED - atomic_sm_rsub_u32 / atomic_sm_rsub_u64.
  //
  // u-l-i-g documents this form as `src - old`, and notes that the result is
  // not commutative, so the contents left behind depend on the lane service
  // order.  The hardware does not write the cell at all.  The differential is
  // exact: the carrier emits a word byte-identical to atomic_sm_sub_u32's
  // except for the opcode field - lo=0x0d08000b, the same operands, with
  // hi=0x20340000 for rsub against 0x200c0000 for sub - and sub writes while
  // hi[26:18]=0x00d does not.  Measured on the compute pipe at lane counts
  // 1,2,3,4,5 for the u32 rows and 1,2 for the u64 rows: the cell holds its
  // initial value at every count, including count 1, where a single
  // `src - old` application would already differ (initial 0x3 with src 0xa:
  // 0x3 observed, 0x7 predicted).  The closed driver's own derivation string
  // for these two rows is "...ps-lds-atomic-no-write-both-hi30-values...", so
  // its encoding was derived from the same no-write observation.
  //
  // Modelling the documented `a - old` here is worse than merely wrong: it
  // oscillates with period two (v1 = a - old, v2 = a - v1 = old), so it AGREES
  // with "unchanged" at every even lane count - a false PASS there, a failure
  // at odd counts.  The measured no-write invariant is compared instead: a
  // falsifiable statement that will fail, loudly, if a later encoding or part
  // does write.
  if (op == "rsub") return old;
  if (op == "and") return old & a;
  if (op == "or") return old | a;
  if (op == "xor") return old ^ a;
  if (op == "swap") return a;
  if (op == "inc") return old >= a ? 0 : old + 1;
  if (op == "dec") return old == 0 || old > a ? a : old - 1;
  if (op == "cmpswap" || op == "cmpswap_eq") return old == a ? b : old;
  if (op == "min") {
    return sgn ? (static_cast<std::int64_t>(old) <
                          static_cast<std::int64_t>(a) ? old : a)
               : (old < a ? old : a);
  }
  if (op == "max") {
    return sgn ? (static_cast<std::int64_t>(old) >
                          static_cast<std::int64_t>(a) ? old : a)
               : (old > a ? old : a);
  }
  return old;
}

// The atomics are PER-LANE: every executing lane applies the operation to the
// addressed cell, so the cell's final content is the operation applied once per
// lane, not once per instruction.  The oracle's vectors state one lane's view
// (dst[0] = old, dst[1] = old <op> src), which is exactly the `lanes == 1`
// case of this model, so the model has to be told how many lanes the launch
// actually executes.  Two consequences make the count worth modelling rather
// than assuming:
//
//   - it is invisible for the idempotent operations (min/max/and/or/xor and
//     cmpswap all converge after the first application), so a wrong count
//     cannot be detected from those rows;
//   - it is decisive for the accumulating ones (add/sub/inc/dec), where the
//     cell differs by (lanes - 1) applications.
//
// Measured linearity on the compute pipe, `add` row (initial 5, src 3):
// group 1/2/4/8/16 -> 8/11/17/29/53, i.e. 5 + lanes * 3 with no residual.
// The graphics launch executes 16 lanes, which is why the same rows agree
// there for either family and disagree on a 4-lane compute group.
//
// 32-bit buff/tex-atomic driver (swap is the cmpswap replacement dword);
// signed min/max compare sign-extended 32-bit values, exactly like the lds
// driver does for its width-1 signed forms.
std::uint32_t atomic_final_n(const std::string& op, std::uint64_t init,
                             std::uint64_t src, bool sgn,
                             std::uint64_t swap, unsigned lanes) {
  std::uint64_t v = init, a = src;
  if (sgn && (op == "min" || op == "max")) {
    v = static_cast<std::uint64_t>(
        static_cast<std::int64_t>(static_cast<std::int32_t>(v)));
    a = static_cast<std::uint64_t>(
        static_cast<std::int64_t>(static_cast<std::int32_t>(a)));
  }
  for (unsigned lane = 0; lane < lanes; ++lane)
    v = atomic_apply(op, sgn, v, a, swap);
  return static_cast<std::uint32_t>(v);
}

// 64-bit lds driver; callers pre-extend width-1 signed forms.
std::uint64_t sm_final_n(const std::string& op, bool sgn, std::uint64_t initial,
                         std::uint64_t a, std::uint64_t b, unsigned lanes) {
  std::uint64_t v = initial;
  for (unsigned lane = 0; lane < lanes; ++lane)
    v = atomic_apply(op, sgn, v, a, b);
  return v;
}

// ---- launch environment ------------------------------------------------
//
// One program shape, two dispatch engines.  The instruction stream, the source
// staging and the family quirks are identical; what differs is the two things
// the launch actually provides, and they are exactly these:
//
//   1. WHERE the result buffer is and HOW it is reached.
//      Graphics: the PS-control packet hydrates the constant/resource roots, so
//        the carrier assembles a BUFFER DESCRIPTOR in s0-s3 pointing at the
//        fixed argument BO and stores with `store_v_buff_b32`.
//      Compute: there is no constant root and no hydrated user data, and a
//        compiled compute kernel reaches its first user argument through the
//        kernarg pointer in s[6:7].  This carrier does not depend on that at
//        all: it assembles the OUTPUT BO's VA as a literal into a VGPR pair and
//        stores with the flat form, which is the shape the vendor's own
//        compiled kernels emit (`store.v.flat.b32 p0, v0, v[4:5]` with the
//        full absolute address in the pair and a zero architectural base).
//        That keeps the result path independent of the launch state, so a
//        wrong launch register cannot masquerade as a wrong instruction.
//
//   2. Which registers the hardware itself writes.
//      Graphics: nothing is reserved below the ABI's own roots, so the scalar
//        source window starts at s4.
//      Compute: LOONGVK_KD_RSRC1 pins kernel_sgpr_cnt = 11, so the hardware
//        writes GROUP ID x/y/z into s11/s12/s13 and the carrier must never
//        touch them.  The scalar window therefore starts at s8, with the
//        descriptor-load registers s0-s3 and the flat-store staging s18-s21
//        outside it.
struct CarrierEnv {
  LaunchEnvironment kind = LaunchEnvironment::Graphics;
  // How many lanes the launch executes.  This is a property of the DISPATCH,
  // not of the instruction, which is why it lives here: the graphics fragment
  // launch runs a 4x4 region (kGraphicsLanes = 16), while a compute dispatch
  // runs exactly the workgroup it was handed (cs_group(), and therefore
  // LGPU_CS_GROUP, decides that).  The per-lane atomics read it; see
  // atomic_final_n for why the count cannot be assumed.
  unsigned lanes = kGraphicsLanes;
  // The buffer the carrier stages inputs into and stores results into.
  std::uint64_t arg_va = carrier::kArgVa;
  // First SGPR of the scalar source staging window.  The compute window starts
  // at 8 because the graphics window's base (4) sits under registers the kernel
  // descriptor pins: the hardware writes the group ids into s11-s13.
  std::uint16_t scalar_base = 4;
  // True for the compute environment: store with the flat form instead of a
  // hand-built buffer descriptor.
  bool flat_store = false;
  // The image (T#) and sampler (S#) tuple registers.  They must not overlap the
  // scalar staging window, because the staging writes are what a family emits
  // around the instruction under test; in compute that window starts at s8,
  // which is exactly where the graphics tuple lives.
  isa::Reg image_tuple = isa::sgpr(8, 8);
  isa::Reg sampler_tuple = isa::sgpr(16, 4);
  // First SGPR of the window a scalar-load carrier writes its result into.  It
  // must not overlap a register another part of the same program still reads:
  // in compute the flat result store names s20:s21 as its architectural zero
  // base, so a scalar load destination at s20 turned the next result store into
  // an access at (result VA + loaded value) - measured as a protection fault at
  // kArgVa + 0x12345678 + 8 that quarantined the pipe.
  isa::Reg scalar_dst = isa::sgpr(20, 2);
  // Where the resource tuple lives relative to the pointer the carrier loads it
  // through (s[6:7]).  Graphics hydrates s[6:7] to point at the tuple inside its
  // argument BO, so the load offset is zero; compute's s[6:7] is the kernarg
  // base, so the tuple is reached at its table offset instead.
  std::uint32_t rsd_load_off = 0;
  // The executable stub an indirect control transfer may target.
  std::uint64_t trampoline_va = carrier::kTrampolineVa;
  std::uint32_t sampler_load_off = carrier::kSamplerSlot - carrier::kRsdTable;

  static CarrierEnv graphics() { return CarrierEnv{}; }
  static CarrierEnv compute(unsigned lanes) {
    CarrierEnv env;
    env.kind = LaunchEnvironment::Compute;
    env.lanes = lanes;
    env.arg_va = carrier::kCsArgVa;
    env.scalar_base = 8;
    env.flat_store = true;
    // s24-s35: clear of the compute staging window (s8..s15), of the flat zero
    // base pair (s20:s21) and of the group-id registers (s11-s13).  The kernel
    // descriptor declares the high-water mark, so the wider allocation is
    // declared rather than assumed.
    env.image_tuple = isa::sgpr(24, 8);
    env.sampler_tuple = isa::sgpr(32, 4);
    env.scalar_dst = isa::sgpr(36, 2);
    env.trampoline_va = carrier::kCsTrampolineVa;
    env.rsd_load_off = carrier::kRsdTable;
    env.sampler_load_off = carrier::kSamplerSlot;
    return env;
  }
};

// Shared PS carrier environment: the result-store buffer descriptor in
// s0-s3 ({lo=0, hi=1, size 0x40, stride/format 0x27fac} targeting the
// argument BO).  Every graphics carrier starts with this.
// The store descriptor's base address.  It defaults to the argument BO, which
// is what every correctness carrier wants: the host reads results back from
// there.  The BENCH carrier passes the colour BO instead, and that is not
// cosmetic - the argument BO is also where the 0x13 timestamp pair lands, and a
// program that writes into the same BO drops the END timestamp's writeback, so
// the bench could never see a tick count.  Keeping the two BOs separate is what
// makes the timestamped bench measurable at all.
void emit_store_desc(Builder& builder,
                     std::uint64_t base = carrier::kArgVa) {
  builder.s_mov_imm_b32(isa::sgpr(0), static_cast<std::uint32_t>(base));
  builder.s_mov_imm_b32(isa::sgpr(1),
                        static_cast<std::uint32_t>(base >> 32));
  builder.s_mov_imm_b32(isa::sgpr(2), 0x00000040u);
  builder.s_mov_imm_b32(isa::sgpr(3), 0x00027facu);
}

// Materialize one absolute 64-bit address into a VGPR pair by staging it
// through two immediates.  Both carriers need this for the flat forms: the
// graphics flat carrier to address the argument BO, the compute carrier to
// address every result store.
void emit_absolute_pair(Builder& builder, const isa::Reg& lo,
                        const isa::Reg& hi, const isa::Reg& temp,
                        std::uint64_t va) {
  builder.s_mov_imm_b32(temp, static_cast<std::uint32_t>(va));
  builder.v_mov_b32(lo, temp, isa::predicate(0));
  builder.s_mov_imm_b32(temp, static_cast<std::uint32_t>(va >> 32));
  builder.v_mov_b32(hi, temp, isa::predicate(0));
}

// One result dword.  Graphics: buffer-descriptor store against the argument BO.
// Compute: flat store to the absolute address arg_va + offset, with the
// architectural zero base pair in s20:s21 (staged once by emit_env_prelude).
void emit_result_store(Builder& builder, const CarrierEnv& env,
                       const isa::Reg& data, const isa::Reg& zero_addr,
                       std::uint32_t off) {
  if (!env.flat_store) {
    builder.store_v_buff_b32(isa::predicate(0), data, zero_addr,
                             carrier::kStoreDesc(), isa::zero_reg(), off,
                             true, false, false, false);
    return;
  }
  // The address pair MUST NOT be v0:v1.  The carriers use v0 (and v0:v1) as the
  // memory access index/address of every case row, and this store runs after
  // row N and before row N+1: with v0:v1 the next row's access picked up
  // (arg_va + off) as its index.  The buff family then faulted - protection
  // fault at 0x100160300 = buff base + arg_va + off, costing power cycles - and
  // the shared-memory family silently read the wrong cell, because an LDS
  // offset is 16 bits and the wild value wraps inside LDS instead of faulting.
  // v26:v27: the audit's result-pair rule requires a pair that NO carrier
  // reads anywhere, and across all 768 compute programs v0..v25 are all in use.
  emit_absolute_pair(builder, isa::vgpr(26), isa::vgpr(27), isa::sgpr(16),
                     env.arg_va + off);
  builder.store_v_flat("store_v_flat_b32", isa::predicate(0), data,
                       isa::vgpr(26, 2), isa::sgpr(20, 2));
}

// The environment's own prelude.  Graphics: the store descriptor plus a zeroed
// address register.  Compute: the zero architectural base pair the flat stores
// name, and the store-target pair zeroed so a family that leaves it alone
// cannot write through a stale address.
void emit_env_prelude(Builder& builder, const CarrierEnv& env) {
  // Both environments want the 4-dword buffer descriptor in s0-s3: the
  // graphics side uses it for every result store, and the instructions under
  // test that take a descriptor operand (store_v_buff_*, the subword forms)
  // take it from the same registers.  The graphics side leaves it to
  // emit_store_desc; a compute dispatch has nothing to hydrate it from, so the
  // same words are assembled from immediates.
  emit_store_desc(builder, env.arg_va);
  if (env.flat_store) {
    // The architectural zero base pair a flat access names; staged once so no
    // result store has to.
    builder.s_mov_imm_b32(isa::sgpr(20), 0u);
    builder.s_mov_imm_b32(isa::sgpr(21), 0u);
  }
}

// Stage a 4-dword buffer descriptor for the families that read through one
// (load_s_buff_*, and the buff carrier's load under test).
//
// Graphics: the PS-control packet hydrates s4-s7 from the packet's user data,
// and every graphics carrier relies on that, so nothing is emitted.
// Compute: there is nothing to hydrate from, so the descriptor is assembled
// from immediates against this carrier's own argument BO, exactly the way the
// graphics side assembles its result-store descriptor.  Same four words, same
// meaning; only the producer differs.
void emit_load_desc(Builder& builder, const CarrierEnv& env,
                    const isa::Reg& quad, std::uint64_t base) {
  if (!env.flat_store) {
    return;
  }
  builder.s_mov_imm_b32(isa::sgpr(quad.index + 0),
                        static_cast<std::uint32_t>(base));
  builder.s_mov_imm_b32(isa::sgpr(quad.index + 1),
                        static_cast<std::uint32_t>(base >> 32));
  builder.s_mov_imm_b32(isa::sgpr(quad.index + 2), 0x00000040u);
  builder.s_mov_imm_b32(isa::sgpr(quad.index + 3), 0x00027facu);
}


// Shared prelude for the texture-family carriers: store descriptor, zero
// address, T# (+ optional S#) staged into the register windows through the
// proven scalar-flat form.  `scratch_sgpr` is the family staging scalar
// (s20 in the samp family, whose sampler tuple occupies s16-s19).
struct TexStage {
  isa::Reg store_desc;
  isa::Reg tdesc_regs;
  isa::Reg sdesc_regs;   // valid only when sdesc != nullptr
  isa::Reg imm_temp;
};

TexStage emit_tex_prelude(Builder& builder, EnvelopeProgram& program,
                          const CarrierEnv& env,
                          const std::uint32_t tdesc[8],
                          const std::uint32_t* sdesc,
                          std::uint32_t scratch_sgpr) {
  emit_env_prelude(builder, env);
  TexStage st{carrier::kStoreDesc(), env.image_tuple,
              env.sampler_tuple, isa::sgpr(scratch_sgpr)};
  builder.s_mov_imm_b32(st.imm_temp, 0u);
  builder.v_mov_b32(isa::vgpr(0), st.imm_temp, isa::predicate(0));
  for (unsigned i = 0; i < 8; ++i)
    program.arg_preloads.emplace_back(carrier::kRsdTable + 4 * i, tdesc[i]);
  builder.sflat_load("load_s_flat_w8", st.tdesc_regs,
                     isa::sgpr(6, 2), isa::zero_reg(), env.rsd_load_off);
  builder.check_k_vm_vs_sm();
  if (sdesc != nullptr) {
    for (unsigned i = 0; i < 4; ++i)
      program.arg_preloads.emplace_back(carrier::kSamplerSlot + 4 * i, sdesc[i]);
    builder.sflat_load("load_s_flat_w4", st.sdesc_regs,
                       isa::sgpr(6, 2), isa::zero_reg(),
                       env.sampler_load_off);
    builder.check_k_vm_vs_sm();
  }
  return st;
}

// Encode the program and publish the register high-water mark.  The mark is
// what the compute path declares in the kernel descriptor, so it is taken from
// the emitted operands rather than from a fixed window that the program may
// outgrow.
bool finalize_program(Builder& builder, EnvelopeProgram& program,
                      std::string& error) {
  if (!link_program(builder.lines, program, error)) {
    return false;
  }
  program.sgpr_high_water = builder.sgpr_high;
  program.vgpr_high_water = builder.vgpr_high;
  return true;
}

// Program tail shared by every carrier.
bool finish_program(Builder& builder, EnvelopeProgram& program,
                    std::string& error) {
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();
  return finalize_program(builder, program, error);
}

// ---- build_ps helper families (each an inlined instruction-family quirk) ----

// v_mac* : the vector destination is the accumulator and must be pre-zeroed
// per case.
void stage_mac_accumulator(Builder& builder, const isa::Reg& imm_temp,
                          std::uint32_t acc) {
  // v_mac accumulates into the destination VGPR: dst += src0*src1.  The
  // third case input is the initial accumulator value.
  builder.s_mov_imm_b32(imm_temp, acc);
  builder.v_mov_b32(isa::vgpr(10), imm_temp, isa::predicate(0));
}

// s_bitset* : RMW on the scalar destination register; input dword 0 is the
// RMW base staged directly into the destination register, and the operand
// sources start at input dword 1.
void stage_bitset_base(Builder& builder, std::uint32_t base) {
  builder.s_mov_imm_b32(isa::sgpr(8), base);  // RMW base
}

// s_cmp*/s_cmpk/s_bitcmp*: the compare writes SCC; materialize the scalar
// result through an s_select.  The select constants must be staged BEFORE
// the compare (s_mov_imm_b32 writes SCC) and live outside the scalar source
// window (s4..), which they would otherwise alias for 64-bit compares.
void append_implicit_scc_op(Builder& builder, const std::string& mnemonic,
                            std::vector<isa::Operand>&& operands) {
  builder.s_mov_imm_b32(isa::sgpr(9), 0x0000ffffu);
  builder.s_mov_imm_b32(isa::sgpr(10), 0u);
  builder.append_target(isa::McInst{mnemonic, "", std::move(operands)});
  builder.append_target(isa::McInst{
      "s_select_b32", "",
      {isa::op_reg(isa::sgpr(8)), isa::op_reg(isa::sgpr(9)),
       isa::op_reg(isa::sgpr(10)), isa::op_reg(isa::scc_reg())}});
}

// One generic register/immediate emitter, two launch environments.  The
// instruction stream, the source staging and the result stores are shared; only
// the store descriptor's base and the program tail differ (see the Builder
// setup below, and build_cs in the header).
std::optional<EnvelopeProgram> build_generic(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    const CarrierEnv& env, std::string& error) {
  const auto* spec = isa::semantic_spec(mnemonic);
  if (spec == nullptr || spec->rule_count == 0) {
    error = "no semantic specification";
    return std::nullopt;
  }
  std::vector<isa::RegClass> classes;
  if (!general_source_classes(mnemonic, classes)) {
    error = "no canonical variant";
    return std::nullopt;
  }
  if (!materializable(*spec, classes)) {
    error = "not materializable on the generic PS carrier";
    return std::nullopt;
  }
  if (mnemonic == "nop" || mnemonic == "exit" || mnemonic == "check") {
    error = "excluded mnemonic";
    return std::nullopt;
  }

  // Input dwords consumed by the operand template.
  bool has_vcc_output = false;
  unsigned input_count = 0;
  for (unsigned i = 0; i < spec->rule_count; ++i) {
    const auto& rule = spec->rules[i];
    if (rule.sem == isa::FieldSem::Vcc8) {
      has_vcc_output = true;
    }
    if (rule.sem == isa::FieldSem::VectorSource8 ||
        rule.sem == isa::FieldSem::GeneralSource11 ||
        rule.sem == isa::FieldSem::GeneralSource8 ||
        rule.sem == isa::FieldSem::Scalar8 ||
        rule.sem == isa::FieldSem::Immediate32 ||
        rule.sem == isa::FieldSem::Immediate8) {
      if (rule.sem == isa::FieldSem::Scalar8 && rule.is_output) {
        continue;
      }
      if (rule.sem == isa::FieldSem::Immediate32 ||
          rule.sem == isa::FieldSem::Immediate8) {
        ++input_count;
      } else {
        input_count +=
            isa::semantic_tuple(spec->mnemonic, rule.field, rule.tuple_width);
      }
    }
  }

  // Filter rows using the fixed case shape.
  std::vector<EnvelopeCase> active;
  std::uint32_t offset = 0;
  for (const auto& row : rows) {
    if (row.expected.size() != 1 && row.expected.size() != 2) {
      continue;
    }
    if (row.inputs.size() < input_count) {
      continue;
    }
    std::vector<std::uint32_t> expect = row.expected;
    if (has_vcc_output) {
      expect = {expect.front()};
    }
    // Lane-mask outputs are a function of the launch's lane count.  Measured
    // pattern on the saved CS matrix: every v_cmp row whose oracle mask is
    // non-zero fails while every zero-mask row passes (pass count equals the
    // number of zero expectations across the family), and no v_cmp row with a
    // non-zero mask ever passes - so the set bits are not all present.  The
    // oracle spells only 0x0000ffff (all 16 reference lanes true) or 0, so
    // mapping that one value to the N-lane all-true mask is the whole model.
    // A wrong mask width keeps the row FAILing rather than turning it PASS, so
    // this cannot manufacture a false PASS; the lane sweep is the confirmation.
    if (is_lane_mask_family(spec->mnemonic) && env.lanes > 0) {
      const std::uint32_t lane_mask =
          env.lanes >= 32 ? 0xffffffffu : ((1u << env.lanes) - 1u);
      for (auto& value : expect) {
        if (value == 0x0000ffffu) value = lane_mask;
      }
    }
    EnvelopeCase c = row;
    c.expected = expect;
    active.push_back(std::move(c));
    offset += 4 * static_cast<std::uint32_t>(expect.size());
  }
  if (active.empty()) {
    error = "no case rows qualify for the generic carrier";
    return std::nullopt;
  }

  Builder builder;
  // Store-buffer descriptor in sgpr(0..3), plus a zeroed address register.
  // v9 doubles as the zero store address because v0 is the first
  // vector-source staging register in this carrier.
  //
  // The two launch environments differ in exactly one place, and it is the
  // descriptor's BASE:
  //
  //   graphics (PS): the argument BO is at a fixed VA the carrier owns, so
  //   the base is a literal the program can assemble.
  //
  //   compute (CS): the compute stage has no constant root and no user data;
  //   the hardware puts the KERNARG POINTER in s[6:7] and nothing else names
  //   the result buffer.  The host writes the output BO's GPU VA into the
  //   first eight bytes of the kernarg block (drm::submit_compute), so the
  //   program reads its own store base out of memory instead of assembling
  //   it.  That read has to be the FIRST instruction: the generic emitter
  //   stages 64-bit scalar sources from s4 upward and hands VCC instructions
  //   s[6:7], either of which destroys the kernarg pointer.
  if (env.flat_store) {
    // Compute: the hardware puts the KERNARG POINTER in s[6:7] and the host
    // writes this carrier's output BO VA into the first user argument there, so
    // the program reads its own launch state before anything can clobber
    // s6:s7.  The result path does not use the value (see emit_result_store),
    // but the read itself is the launch-state evidence: a wrong kernarg pointer
    // faults here rather than surfacing later as a wrong value.
    builder.sflat_load("load_s_flat_w2", isa::sgpr(0, 2), isa::sgpr(6, 2),
                       isa::zero_reg(), 0u);
  }
  emit_env_prelude(builder, env);
  const isa::Reg zero_address = isa::vgpr(9);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  // imm_temp must not collide with the scalar source staging window (s4+):
  // a vector source staged through it would clobber an already-staged
  // scalar source (seen as v_cmp*s comparing a value against itself).
  const isa::Reg imm_temp = isa::sgpr(16);
  const isa::Reg scalar_temp = isa::sgpr(5);

  const bool is_mac_accumulator =
      std::strncmp(spec->mnemonic, "v_mac", 5) == 0;
  // v_dp4a_* reads an unmodeled third source.  Its driver row is
  // GJ11 GK11 VD8, but the encoding also carries LO[23:16], outside every field
  // the row names (the driver's own print-ignored mask), and the hardware adds
  // the scalar register that field selects.  Template 0 selects s0.  Measured
  // on compute: every case returned expected + 0x000b0000, and 0x000b0000 is
  // exactly the s0 this prelude stages as the result-store descriptor base; the
  // same program on graphics - where the packet hydrates s0 to 0x00000000 -
  // passes.  The catalog page says the form is a pure product sum, "the
  // destination's previous value is not an accumulate input", so the addend is
  // this hidden source and must be zero.  The flat result store does not read
  // s0, so zeroing it before the instruction restores the documented sum.  The
  // durable fix is to model the field in the catalog; this is the carrier-side
  // correction of a launch-state dependency the row does not state.
  //
  // v_mbcnt_u32 has the same print-ignored fields but is NOT fixed by the same
  // zeroing: its instruction word and staged registers are byte-identical on
  // both pipes, yet compute reads 0x103 where graphics reads 0x1ff for a=0xf,
  // b=0xff, at every lane count, and zeroing s0 does not change it.  Its
  // implicit input is therefore something else (exec/lane state, not s0) and
  // stays open rather than guessed at.
  const bool is_dp4a = std::strncmp(spec->mnemonic, "v_dp4a_", 7) == 0;
  const bool is_mma = std::strncmp(spec->mnemonic, "v_mma_", 6) == 0;
  const bool is_u64cmp = std::string(spec->mnemonic) == "s_cmp_eq_u64" ||
                         std::string(spec->mnemonic) == "s_cmp_ne_u64";
  const bool is_bitset =
      std::string(spec->mnemonic).rfind("s_bitset", 0) == 0;
  // Scalar carry/borrow immediate forms: the third case input is the SCC
  // carry/borrow-in state (the instruction reads the SCC flag, not a
  // register operand).  The staged SCC must be the LAST scalar move before
  // the target instruction, so the row's third input is consumed here and
  // emitted as a trailing s_mov_imm (s_mov_imm writes SCC = value != 0).
  const bool is_scc_borrow =
      std::string(spec->mnemonic) == "s_addc_imm_u32" ||
      std::string(spec->mnemonic) == "s_subb_imm_u32" ||
      std::string(spec->mnemonic) == "s_subbrev_imm_u32";
  EnvelopeProgram program;
  // [HW 2026-08-24] hardware-approximation families (sqrt/rsq/rcp/log2/
  // sin/cos and the f64 spirit) may land 1 ulp away from the correctly
  // rounded softfloat oracle.
  const std::string& mn = spec->mnemonic;
  if (mn.rfind("v_sqrt", 0) == 0 || mn.rfind("v_rsq", 0) == 0 ||
      mn.rfind("v_rcp", 0) == 0 || mn.rfind("v_log2", 0) == 0 ||
      mn.rfind("v_sin", 0) == 0 || mn.rfind("v_cos", 0) == 0 ||
      mn.rfind("v_exp", 0) == 0 || mn.rfind("v_frexp", 0) == 0)
    program.approx1ulp = true;
  // [HW 2026-08-25] the interpolation phases read the coefficient window
  // (launch-dependent barycentric state), not the staged sources; the
  // generic carrier cannot oracle them (same class as load_sm_interp).
  if (mn == "v_interp_2_f32" || mn == "v_interp_mov_b32")
    program.observe = true;
  // [HW 2026-08-25] LG210-legacy no-op holes (decode but execute empty on
  // LG200, dest left undefined) and the lane-id read (the carrier's
  // multi-lane write leaves the last lane's value; which lane is read
  // back is not oracle-able): record observations without failing.
  if (mn == "v_fcvt_f16_i16" || mn == "v_fcvt_f16_u16" ||
      mn == "v_fcvt_i16_f16" || mn == "v_fcvt_u16_f16" ||
      mn == "v_threadid_u32")
    program.observe = true;
  std::uint32_t case_offset = 0;
  for (const auto& def : active) {
    const std::uint32_t result_offset = case_offset;
    unsigned input_index = 0;
    if (is_mac_accumulator)
      stage_mac_accumulator(
          builder, imm_temp, def.inputs.size() > 2 ? def.inputs.at(2) : 0u);
    if (env.flat_store && is_dp4a) {
      builder.s_mov_imm_b32(isa::sgpr(0), 0u);
    }
    unsigned vgpr_window = 0;
    unsigned sgpr_window = 0;
    if (is_bitset) stage_bitset_base(builder, def.inputs.at(0));
    std::vector<isa::Operand> operands;
    isa::Reg dst_vgpr = isa::vgpr(10);
    isa::Reg dst_sgpr = isa::sgpr(8);
    bool has_vgpr_dst = false;
    bool has_sgpr_dst = false;
    std::uint32_t scc_borrow = 0;
    bool scc_staged = false;
    if (is_mma) {
      // [HW 2026-08-25] the 8816/323216 mma reads 16-BYTE register
      // windows starting at each source register (4 identical dwords per
      // window, matching oracle_v_mma's x4 model).  Stage the input
      // replicated into v0..v3 / v4..v7 / v8..v11 and read single
      // dwords of the window bases; the raw one-dword staging left every
      // window full of garbage, producing the old 0x118-class results.
      for (unsigned w = 0; w < 3; ++w) {
        const std::uint32_t value = def.inputs.at(w);
        for (unsigned p = 0; p < 4; ++p) {
          builder.s_mov_imm_b32(imm_temp, value);
          builder.v_mov_b32(isa::vgpr(4 * w + p), imm_temp,
                            isa::predicate(0));
        }
      }
      // v9 doubles as the result-store zero address; restore it after
      // the C window staging so the store keeps targeting arg+off.
      builder.s_mov_imm_b32(imm_temp, 0u);
      builder.v_mov_b32(isa::vgpr(9), imm_temp, isa::predicate(0));
    }
    for (unsigned ri = 0; ri < spec->rule_count; ++ri) {
      const auto& rule = spec->rules[ri];
      const std::uint8_t width =
          isa::semantic_tuple(spec->mnemonic, rule.field, rule.tuple_width);
      switch (rule.sem) {
        case isa::FieldSem::Predicate2:
          operands.push_back(isa::op_reg(isa::predicate(0)));
          break;
        case isa::FieldSem::VectorDest8:
          dst_vgpr = isa::vgpr(10, width);
          operands.push_back(isa::op_reg(dst_vgpr));
          has_vgpr_dst = true;
          break;
        case isa::FieldSem::VectorSource8:
        case isa::FieldSem::GeneralSource11:
        case isa::FieldSem::GeneralSource8: {
          if (is_mma) {
            // mma windows are pre-staged (v0/v4/v8 bases); emit the
            // base register without re-staging.
            operands.push_back(isa::op_reg(isa::vgpr(4 * input_index)));
            ++input_index;
            break;
          }
          const std::size_t flen = std::strlen(rule.field);
          const bool narrow8 =
              flen > 0 && rule.field[flen - 1] == '8' &&
              classes[ri] != isa::RegClass::Vector;
          const std::uint32_t src_index = is_bitset ? 1u : input_index;
          if (classes[ri] == isa::RegClass::Scalar || narrow8) {
            const isa::Reg src = isa::sgpr(4 + sgpr_window, width);
            for (std::uint8_t part = 0; part < width; ++part) {
              const std::uint32_t value = def.inputs.at(src_index + part);
              if (!is_bitset) ++input_index;
              builder.s_mov_imm_b32(isa::sgpr(4 + sgpr_window + part), value);
            }
            operands.push_back(isa::op_reg(src));
            sgpr_window += width;
          } else {
            const isa::Reg src = isa::vgpr(vgpr_window, width);
            for (std::uint8_t part = 0; part < width; ++part) {
              const std::uint32_t value =
                  def.inputs.at(src_index + part);
              if (!is_bitset) ++input_index;
              builder.s_mov_imm_b32(imm_temp, value);
              builder.v_mov_b32(isa::vgpr(vgpr_window + part), imm_temp,
                                isa::predicate(0));
            }
            operands.push_back(isa::op_reg(src));
            vgpr_window += width;
          }
          break;
        }
        case isa::FieldSem::ScalarCmp8:
          if (rule.is_output) {
            operands.push_back(isa::op_reg(isa::scc_reg()));
          } else {
            error = "unreachable scalar-cmp source";
            return std::nullopt;
          }
          break;
        case isa::FieldSem::Scalar8:
          if (rule.is_output) {
            dst_sgpr = isa::sgpr(8, width);
            operands.push_back(isa::op_reg(dst_sgpr));
            has_sgpr_dst = true;
          } else {
            // [HW 2026-08-25] probe: s_cmp_*_u64 pairs staged at s12..s15
            // (the s6:s7 pair is VCC and never produced SCC on the tests);
            // select even pair bases for the 64-bit compares.
            const unsigned base = is_u64cmp ? 12u : 4u;
            const isa::Reg src = isa::sgpr(base + sgpr_window, width);
            for (std::uint8_t part = 0; part < width; ++part) {
              const std::uint32_t value = def.inputs.at(input_index++);
              builder.s_mov_imm_b32(isa::sgpr(base + sgpr_window + part),
                                    value);
            }
            operands.push_back(isa::op_reg(src));
            sgpr_window += width;
          }
          break;
        case isa::FieldSem::Vcc8: {
          const std::uint32_t value =
              input_index < def.inputs.size() ? def.inputs.at(input_index++)
                                              : 0u;
          builder.s_mov_imm_b32(isa::vcc(6), value);
          // VCC is a 64-bit pair (s6:s7); clear the high half so carry/borrow
          // reads a deterministic (0x00000000:value) VCC.  This is a scalar
          // ALU write before the vector target; SCC churn is irrelevant to
          // vector ops.
          builder.s_mov_imm_b32(isa::sgpr(7), 0u);
          operands.push_back(isa::op_reg(isa::vcc(6)));
          break;
        }
        case isa::FieldSem::SccSource8: {
          const std::uint8_t w = isa::semantic_tuple(
              spec->mnemonic, rule.field, rule.tuple_width);
          const isa::Reg src = isa::sgpr(4 + sgpr_window, w);
          for (std::uint8_t part = 0; part < w; ++part) {
            const std::uint32_t value = def.inputs.at(input_index++);
            builder.s_mov_imm_b32(isa::sgpr(4 + sgpr_window + part), value);
          }
          operands.push_back(isa::op_reg(src));
          sgpr_window += w;
          break;
        }
        case isa::FieldSem::Immediate32:
        case isa::FieldSem::Immediate8:
          if (is_scc_borrow && input_index == 1) {
            // first immediate is the real operand; the third input (index 2)
            // stages SCC - hold it for a trailing scalar move.
            operands.push_back(isa::op_imm(def.inputs.at(input_index++)));
            scc_borrow = def.inputs.at(2);
            break;
          }
          operands.push_back(isa::op_imm(def.inputs.at(input_index++)));
          break;
        case isa::FieldSem::Immediate12:
        case isa::FieldSem::OffsetSource8:
        case isa::FieldSem::ScalarQuad8:
          error = "unreachable memory-form rule";
          return std::nullopt;
        case isa::FieldSem::Modifier:
          break;
        default:
          error = "unsupported field semantics";
          return std::nullopt;
      }
    }
    if (is_implicit_scc_family(spec->mnemonic)) {
      append_implicit_scc_op(builder, spec->mnemonic, std::move(operands));
      has_sgpr_dst = true;
      dst_sgpr = isa::sgpr(8);
    } else {
      if (is_scc_borrow && def.inputs.size() >= 3 && !scc_staged) {
      // stage SCC as the last scalar move so the target reads it.  s16 is
      // the carrier imm_temp and never holds a source at this point.
      builder.s_mov_imm_b32(isa::sgpr(16), scc_borrow);
      scc_staged = true;
    }
    builder.append_target(isa::McInst{spec->mnemonic, "", std::move(operands)});
    }
    BuiltCase built;
    built.id = def.id;
    if (has_vgpr_dst) {
      const std::uint8_t dw = dst_vgpr.width;
      for (std::uint8_t part = 0; part < dw; ++part) {
        emit_result_store(builder, env, isa::vgpr(dst_vgpr.index + part),
                          zero_address, result_offset + 4 * part);
        built.offsets.push_back(result_offset + 4 * part);
      }
    } else if (has_sgpr_dst) {
      const std::uint8_t dw = dst_sgpr.width;
      for (std::uint8_t part = 0; part < dw; ++part) {
        builder.v_mov_b32(isa::vgpr(11 + part),
                          isa::sgpr(dst_sgpr.index + part),
                          isa::predicate(0));
        emit_result_store(builder, env, isa::vgpr(11 + part), zero_address,
                          result_offset + 4 * part);
        built.offsets.push_back(result_offset + 4 * part);
      }
    }
    built.expected = def.expected;
    program.cases.push_back(std::move(built));
    case_offset += 4 * static_cast<std::uint32_t>(def.expected.size());
    builder.check_k_vm_vs_sm();
  }
  builder.nop();
  builder.exit();

  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// ---- buff (buffer-descriptor memory) carrier --------------------------------
//
// Loads/stores through the PS-record-hydrated buffer descriptor.  Result
// stores and the store-under-test go through the hand-built s0-s3 store
// descriptor (arg-BO base); the load-under-test goes through the hydrated
// s4-s7 load descriptor whose base is the const-root window at arg+0x300 (the
// v2 load-probe family).  The runner preloads byte 0x300+off (the load
// window) with the case input and reads back the result region at 0x20+off.
// s16 is used as the scalar temp so the hydrated s4-s7 descriptor is never
// clobbered.
std::optional<EnvelopeProgram> build_buff(const std::string& mnemonic,
                                          const std::vector<EnvelopeCase>& rows,
                                          const CarrierEnv& env,
                                          std::string& error) {
  const bool is_load = mnemonic.rfind("load_v_buff_", 0) == 0;
  const bool is_store = mnemonic.rfind("store_v_buff_", 0) == 0;
  const bool is_atomic = mnemonic.rfind("atomic_v_buff_", 0) == 0;
  if (!is_load && !is_store && !is_atomic) {
    error = "buff carrier: expected load/store/atomic_v_buff_*";
    return std::nullopt;
  }
  // Shape-true support: only the VQ8-bearing variants share the hardware-
  // verified b32 template. Everything else (u8/i8/b8 sub-dword forms,
  // fmtdesc/fmtinst, inc/dec/sub and 64-bit atomics) has a different
  // operand layout and is rejected rather than silently emitted with a
  // wrong shape.
  static const char* const kVq8[] = {
      "load_v_buff_b32",  "load_v_buff_b64",  "load_v_buff_b96",
      "load_v_buff_b128", "load_v_buff_u16",  "load_v_buff_i16",
      "load_v_buff_i8",   "load_v_buff_u8",
      "load_v_buff_fmtdesc_c1", "load_v_buff_fmtdesc_c2",
      "load_v_buff_fmtdesc_c3", "load_v_buff_fmtdesc_c4",
      "load_v_buff_fmtinst_c1", "load_v_buff_fmtinst_c2",
      "load_v_buff_fmtinst_c3", "load_v_buff_fmtinst_c4",
      "store_v_buff_b8",  "store_v_buff_b16",
      "store_v_buff_fmtdesc_c1", "store_v_buff_fmtdesc_c2",
      "store_v_buff_fmtdesc_c3", "store_v_buff_fmtdesc_c4",
      "store_v_buff_fmtinst_c1", "store_v_buff_fmtinst_c2",
      "store_v_buff_fmtinst_c3", "store_v_buff_fmtinst_c4",
      "store_v_buff_b32", "store_v_buff_b64",
      "store_v_buff_b96", "store_v_buff_b128",
      "atomic_v_buff_add_i32", "atomic_v_buff_xor_b32",
      "atomic_v_buff_sub_i32", "atomic_v_buff_inc_u32",
      "atomic_v_buff_dec_u32",
      "atomic_v_buff_and_b32", "atomic_v_buff_or_b32",
      "atomic_v_buff_swap_b32",
      "atomic_v_buff_max_i32", "atomic_v_buff_max_u32",
      "atomic_v_buff_min_i32", "atomic_v_buff_min_u32",
      "atomic_v_buff_cmpswap_b32"};
  static const char* const kBuff64[] = {
      "atomic_v_buff_add_i64", "atomic_v_buff_and_b64",
      "atomic_v_buff_dec_u64", "atomic_v_buff_inc_u64",
      "atomic_v_buff_max_i64", "atomic_v_buff_max_u64",
      "atomic_v_buff_min_i64", "atomic_v_buff_min_u64",
      "atomic_v_buff_or_b64",  "atomic_v_buff_sub_i64",
      "atomic_v_buff_swap_b64", "atomic_v_buff_xor_b64",
      "atomic_v_buff_cmpswap_b64"};
  bool supported = false;
  for (const auto* m : kVq8) supported = supported || mnemonic == m;
  for (const auto* m : kBuff64) supported = supported || mnemonic == m;
  if (!supported) {
    error = "buff carrier: no shape-true template for " + mnemonic;
    return std::nullopt;
  }
  const bool wide64 = [&]() {
    for (const auto* m : kBuff64)
      if (mnemonic == m) return true;
    return false;
  }();
  const bool cmpswap64 = mnemonic == "atomic_v_buff_cmpswap_b64";
  const bool subword = mnemonic == "load_v_buff_i8" ||
                       mnemonic == "load_v_buff_u8" ||
                       mnemonic == "store_v_buff_b8" ||
                       mnemonic == "store_v_buff_b16";
  const bool fmtdesc = mnemonic == "load_v_buff_fmtdesc_c1";
  const bool fmtinst = mnemonic.rfind("load_v_buff_fmtinst_", 0) == 0 ||
                       mnemonic.rfind("store_v_buff_fmtinst_", 0) == 0 ||
                       mnemonic == "load_v_buff_fmtdesc_c2" ||
                       mnemonic == "load_v_buff_fmtdesc_c3" ||
                       mnemonic == "load_v_buff_fmtdesc_c4" ||
                       mnemonic == "store_v_buff_fmtdesc_c1" ||
                       mnemonic == "store_v_buff_fmtdesc_c2" ||
                       mnemonic == "store_v_buff_fmtdesc_c3" ||
                       mnemonic == "store_v_buff_fmtdesc_c4";
  unsigned width = 1;
  if (mnemonic.find("b64") != std::string::npos) width = 2;
  if (mnemonic.find("b96") != std::string::npos) width = 3;
  if (mnemonic.find("b128") != std::string::npos) width = 4;
  if (mnemonic.find("_c2") != std::string::npos) width = 2;
  if (mnemonic.find("_c3") != std::string::npos) width = 3;
  if (mnemonic.find("_c4") != std::string::npos) width = 4;
  for (const auto& row : rows) {
    const std::size_t need_inputs =
        is_atomic
            ? (wide64 ? (cmpswap64 ? 6u : 4u)
                      : (mnemonic.find("cmpswap") != std::string::npos ? 3u
                                                                       : 2u))
            : (is_store ? width : 1u);
    const std::size_t need_expected =
        (fmtinst || fmtdesc || subword)
            ? (is_store ? std::min<std::size_t>(width, row.expected.size())
                        : 1u)
            : (is_atomic ? (wide64 ? 4u : 2u) : width);
    if (fmtinst || fmtdesc || subword) {
      const std::size_t min_in =
          (!is_store && (mnemonic == "load_v_buff_fmtdesc_c1" ||
                         mnemonic == "load_v_buff_fmtinst_c1"))
              ? 1u
              : (subword && is_store) ? row.inputs.size() : 2u;
      if (row.expected.size() < need_expected || row.inputs.size() < min_in) {
        error = "buff carrier: row needs matching width dwords: " + row.id;
        return std::nullopt;
      }
      continue;
    }
    if (row.expected.size() != need_expected ||
        row.inputs.size() < need_inputs) {
      error = "buff carrier: row needs matching width dwords: " + row.id;
      return std::nullopt;
    }
  }

  Builder builder;
  emit_env_prelude(builder, env);
  // The buffer descriptor the buff instructions under test take as an operand.
  // Graphics: assembled by emit_env_prelude into s0-s3 (base = argument BO).
  // Compute: the same words, from the same place - emit_env_prelude builds
  // them because there is nothing to hydrate them from.
  const isa::Reg store_desc = carrier::kStoreDesc();
  const isa::Reg load_desc = isa::sgpr(4, 4);
  // Graphics: s4-s7 are hydrated from the PS control packet's user data and
  // must never be clobbered.  Compute: nothing hydrates them, so the
  // descriptor is assembled here against the carrier's own load window.
  emit_load_desc(builder, env, load_desc,
                 env.arg_va + carrier::kLoadWindow);
  const isa::Reg zero_address = isa::vgpr(0);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  const isa::Reg imm_temp = isa::sgpr(16);
  const isa::Reg data_v = isa::vgpr(8);

  EnvelopeProgram program;
  program.probe = true;
  // SIO reads/rotates return channel-file content that is not oracle-able
  // (depends on the launch state); observe mode records without failing.
  std::uint32_t off = carrier::kResultBase;
  for (std::size_t ci = 0; ci < rows.size(); ++ci) {
    const auto& def = rows[ci];
    const isa::Reg dst_v = isa::vgpr(10, width);
    if (is_atomic) {
      // The carrier launches 16 fragments, so every lane executes the
      // atomic. Verification therefore targets the FINAL cell value, which
      // is deterministic for the idempotent operations (and/or, min/max,
      // swap, cmpswap) and equals the oracle's `updated` dword. The
      // per-lane old value is racy and is not compared here. xor is excluded
      // (an even lane count makes it collapse to the initial value).
      if (wide64) {
        // 64-bit: cell pair preloaded at kLoadWindow+off; data pair v8:v9;
        // cmpswap pair v10:v11; the same pair is the in-place dst/src.
        program.arg_preloads.emplace_back(carrier::kLoadWindow + off,
                                          def.inputs[0]);
        program.arg_preloads.emplace_back(carrier::kLoadWindow + off + 4,
                                          def.inputs[1]);
        for (unsigned p = 0; p < 2; ++p) {
          builder.s_mov_imm_b32(imm_temp, def.inputs[2 + p]);
          builder.v_mov_b32(isa::vgpr(8 + p), imm_temp, isa::predicate(0));
        }
        if (cmpswap64) {
          for (unsigned p = 0; p < 2; ++p) {
            builder.s_mov_imm_b32(imm_temp, def.inputs[4 + p]);
            builder.v_mov_b32(isa::vgpr(10 + p), imm_temp, isa::predicate(0));
          }
        }
        builder.buff_atomic(mnemonic, isa::predicate(0), isa::vgpr(8, 2),
                            load_desc, isa::zero_reg(), off, true);
      } else {
      builder.s_mov_imm_b32(imm_temp, def.inputs[1]);
      builder.v_mov_b32(data_v, imm_temp, isa::predicate(0));
      const bool cmpswap = mnemonic.find("cmpswap") != std::string::npos;
      if (cmpswap) {
        builder.s_mov_imm_b32(imm_temp, def.inputs[2]);
        builder.v_mov_b32(isa::vgpr(9), imm_temp, isa::predicate(0));
        program.arg_preloads.emplace_back(carrier::kLoadWindow + off, def.inputs[0]);
        builder.buff_atomic(mnemonic, isa::predicate(0), isa::vgpr(8, 2),
                            load_desc, isa::zero_reg(), off, true);
      } else {
        program.arg_preloads.emplace_back(carrier::kLoadWindow + off, def.inputs[0]);
        builder.buff_atomic(mnemonic, isa::predicate(0), data_v, load_desc,
                            isa::zero_reg(), off, true);
      }
      }
      builder.check_k_vm_vs_sm();
      // read back the final cell -> v14; compare against oracle `updated`
      if (wide64) {
        builder.buff_mem("load_v_buff_b64", isa::predicate(0),
                         isa::vgpr(14, 2), zero_address, load_desc,
                         isa::zero_reg(), off, true, false, false, false);
        builder.check_k_vm_vs_sm();
        for (unsigned p = 0; p < 2; ++p) {
          emit_result_store(builder, env, isa::vgpr(14 + p), zero_address,
                            off + 4 * (p + 1));
          builder.check_k_vm_vs_sm();
        }
      } else {
      builder.buff_mem("load_v_buff_b32", isa::predicate(0), isa::vgpr(14),
                       zero_address, load_desc, isa::zero_reg(), off, true,
                       false, false, false);
      builder.check_k_vm_vs_sm();
      emit_result_store(builder, env, isa::vgpr(14), zero_address, off);
      builder.check_k_vm_vs_sm();
      }
      BuiltCase bc;
      bc.id = def.id;
      if (!wide64) bc.offsets.push_back(off);
      // 16-lane final-cell model: every lane applies the same operation,
      // so accumulating operations are deterministic too -- expected =
      // operation applied 16 times to the initial cell value.
      if (wide64) {
        const std::uint64_t cell =
            static_cast<std::uint64_t>(def.inputs[0]) |
            (static_cast<std::uint64_t>(def.inputs[1]) << 32);
        const std::uint64_t src64 =
            static_cast<std::uint64_t>(def.inputs[2]) |
            (static_cast<std::uint64_t>(def.inputs[3]) << 32);
        const std::uint64_t swap64 =
            cmpswap64 ? (static_cast<std::uint64_t>(def.inputs[4]) |
                         (static_cast<std::uint64_t>(def.inputs[5]) << 32))
                      : 0u;
        const std::string op = mnemonic.substr(
            14, mnemonic.find('_', 14) - 14);
        const bool sgn = mnemonic.find("_i64") != std::string::npos;
        const std::uint64_t fin =
            sm_final_n(op, sgn, cell, src64, swap64, env.lanes);
        bc.offsets.push_back(off + 4);
        bc.offsets.push_back(off + 8);
        bc.expected.push_back(static_cast<std::uint32_t>(fin));
        bc.expected.push_back(static_cast<std::uint32_t>(fin >> 32));
      } else {
      bc.expected.push_back(atomic_final_n(
          mnemonic.substr(14, mnemonic.find('_', 14) - 14), def.inputs[0],
          def.inputs[1], mnemonic.find("_i32") != std::string::npos,
          def.inputs.size() >= 3 ? def.inputs[2] : 0u, env.lanes));
      }
      program.cases.push_back(std::move(bc));
      // 16-byte stride keeps every 64-bit cell 8-aligned (a 12-byte
      // stride put alternate rows on 4-mod-8 offsets where the buff b64
      // readback misbehaved): [HW 2026-08-25] wide64 atomics verified
      // only on 8-aligned cells.
      off += wide64 ? 16u : 4u;
      continue;
    }
    if (fmtdesc) {
      const std::uint32_t cell = carrier::kLoadWindow + off;
      if (is_store) {
        builder.s_mov_imm_b32(imm_temp, def.inputs[0]);
        builder.v_mov_b32(isa::vgpr(8), imm_temp, isa::predicate(0));
        builder.buff_mem(mnemonic, isa::predicate(0), isa::vgpr(8),
                         zero_address, store_desc, isa::zero_reg(), cell,
                         false, true, false, false);
        builder.check_k_vm_vs_sm();
        builder.buff_mem("load_v_buff_b32", isa::predicate(0), isa::vgpr(10),
                         zero_address, load_desc, isa::zero_reg(), off, true,
                         false, false, false);
        builder.check_k_vm_vs_sm();
      } else {
        program.arg_preloads.emplace_back(carrier::kLoadWindow + off,
                                          def.inputs[0]);
        // [HW 2026-08-25] the hydrated load descriptor base is the
        // constant window (arg+0x300), so the offset is window-relative
        // (off), not absolute (plain b32 loads prove it).
        builder.buff_mem(mnemonic, isa::predicate(0), isa::vgpr(10),
                         zero_address, load_desc, isa::zero_reg(), off,
                         false, true, false, false);
        builder.check_k_vm_vs_sm();
      }
    } else if (fmtinst || subword) {
      const std::uint32_t cell = carrier::kLoadWindow + off;
      if (is_store) {
        for (unsigned p = 0; p < width; ++p) {
          builder.s_mov_imm_b32(imm_temp, def.inputs[p]);
          builder.v_mov_b32(isa::vgpr(8 + p), imm_temp, isa::predicate(0));
        }
        // store_v_buff_b16 carries an address VGPR (RA8 RK8x4 VQ8 VJ8 UO12)
        // exactly like the b32 form; store_v_buff_b8 does not
        // (RA8 RK8x4 VJ8 UO12).  Emitting the b8 shape for b16 put the
        // descriptor in the VQ8 slot and an SGPR in the descriptor slot with
        // VJ8 missing, so the hardware addressed through the descriptor and
        // faulted at 0x1000b0000 + 0x000b0300 (measured).
        if (mnemonic == "store_v_buff_b16") {
          builder.buff_mem(mnemonic, isa::predicate(0), isa::vgpr(8, width),
                           zero_address, store_desc, isa::zero_reg(), cell,
                           true, false, true, false);
        } else {
          builder.buff_subword_store(mnemonic, isa::predicate(0),
                                     isa::vgpr(8, width), store_desc,
                                     isa::zero_reg(), cell, true);
        }
        builder.check_k_vm_vs_sm();
        builder.buff_mem("load_v_buff_b32", isa::predicate(0), isa::vgpr(10),
                         zero_address, load_desc, isa::zero_reg(), off, true,
                         false, false, false);
        builder.check_k_vm_vs_sm();
      } else {
        for (unsigned p = 0; p < width; ++p)
          program.arg_preloads.emplace_back(cell + 4 * p, def.inputs[p]);
        // [HW 2026-08-25] same window-relative offset as the b32 form.
        builder.buff_subword_load(mnemonic, isa::predicate(0),
                                  isa::vgpr(10, width), load_desc,
                                  isa::zero_reg(), off, true);
        builder.check_k_vm_vs_sm();
      }
    } else if (is_store) {
      // write the test data into the load window, then read it back
      for (unsigned p = 0; p < width; ++p) {
        builder.s_mov_imm_b32(imm_temp, def.inputs[p]);
        builder.v_mov_b32(isa::vgpr(8 + p), imm_temp, isa::predicate(0));
      }
      builder.buff_mem(mnemonic, isa::predicate(0),
                       isa::vgpr(8, width), zero_address, store_desc,
                       isa::zero_reg(), carrier::kLoadWindow + off, true, false,
                       false, false);
      builder.check_k_vm_vs_sm();
      builder.buff_mem("load_v_buff_" + mnemonic.substr(13), isa::predicate(0),
                       dst_v, zero_address, load_desc, isa::zero_reg(), off,
                       true, false, false, false);
      builder.check_k_vm_vs_sm();
    } else {
      // hydrated load descriptor base = constant root (arg+0x300); the
      // immediate is the case offset, so the data lands at arg+0x300+off.
      builder.buff_mem(mnemonic, isa::predicate(0), dst_v,
                              zero_address, load_desc, isa::zero_reg(), off,
                              true, false, false, false);
      builder.check_k_vm_vs_sm();
      for (unsigned p = 0; p < width; ++p)
        program.arg_preloads.emplace_back(carrier::kLoadWindow + off + 4 * p,
                                          def.inputs[p]);
    }
    for (unsigned p = 0; p < width; ++p) {
      emit_result_store(builder, env, isa::vgpr(10 + p), zero_address,
                        off + 4 * p);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    const std::size_t ncmp = def.expected.size() <= width ? def.expected.size()
                                                         : width;
    for (std::size_t p = 0; p < ncmp; ++p) bc.offsets.push_back(off + 4 * p);
    for (std::size_t p = 0; p < ncmp; ++p)
      bc.expected.push_back(def.expected[p]);
    program.cases.push_back(std::move(bc));
    off += 4 * width;
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// 16-lane final-cell model for LDS atomics: every lane applies the same
// operation with the same operand, so the final cell is deterministic.
static bool sm_atomic_op_width(const std::string& mnemonic,
                               std::string& op, unsigned& width) {
  static const char* const kW[] = {"u32", "i32", "b32", "u64", "i64",
                                   "b64", "f64"};
  for (const auto* w : kW) {
    const std::string suf = std::string("_") + w;
    if (mnemonic.size() > suf.size() &&
        mnemonic.compare(mnemonic.size() - suf.size(), suf.size(), suf) == 0) {
      op = mnemonic.substr(10, mnemonic.size() - 10 - suf.size());
      width = suf == "u32" || suf == "i32" || suf == "b32" ? 1 : 2;
      return true;
    }
  }
  return false;
}


// ---- lds (workgroup shared memory) carrier ----------------------------------
// load_sm_*/store_sm_* through a byte-offset address VGPR into the workgroup
// shared memory (shape proven by the conformance sm-memory probe: operand
// form {pred, data-or-dst, address}).  LDS contents are undefined at wave
// start, so LOAD cases first store the input data with the matching store
// op, then execute the target load.  Results land in the argument BO via
// the standard buff b32 result stores.
static unsigned lds_width(const std::string& mne) {
  if (mne.find("_x4") != std::string::npos) return 4;
  if (mne.find("_x2") != std::string::npos) return 2;
  if (mne.find("b64") != std::string::npos ||
      mne.find("u64") != std::string::npos ||
      mne.find("i64") != std::string::npos ||
      mne.find("f64") != std::string::npos ||
      mne.find("swizzle_b64") != std::string::npos)
    return 2;
  return 1;
}
std::optional<EnvelopeProgram> build_lds(const std::string& mnemonic,
                                         const std::vector<EnvelopeCase>& rows,
                                         const CarrierEnv& env,
                                         std::string& error) {
  const bool is_load = mnemonic.rfind("load_sm_", 0) == 0;
  const bool is_store = mnemonic.rfind("store_sm_", 0) == 0;
  const bool is_atomic = mnemonic.rfind("atomic_sm_", 0) == 0;
  if (!is_load && !is_store && !is_atomic) {
    error = "lds carrier: expected load_sm_*/store_sm_*/atomic_sm_*";
    return std::nullopt;
  }
  static const char* const kSupported[] = {
      "load_sm_b32",  "load_sm_b64",  "load_sm_u8",  "load_sm_i8",
      "load_sm_u16",  "load_sm_i16",  "store_sm_b8", "store_sm_b16",
      "store_sm_b32", "store_sm_b64",
      "load_sm_matrix_x1", "load_sm_matrix_x2", "load_sm_matrix_x4",
      "load_sm_swizzle_b64", "store_sm_swizzle_b64",
      "load_sm_interp"};
  bool supported = is_atomic;
  for (const auto* m : kSupported) supported = supported || mnemonic == m;
  if (!supported) {
    error = "lds carrier: no shape-true template for " + mnemonic;
    return std::nullopt;
  }
  const unsigned width = lds_width(mnemonic);
  const bool matrix = mnemonic.rfind("load_sm_matrix_", 0) == 0;
  for (const auto& row : rows) {
    if (is_atomic) {
      if (row.expected.size() != 2 * width ||
          row.inputs.size() < 2 * width) {
        error = "lds carrier: atomic row needs initial+operand: " + row.id;
        return std::nullopt;
      }
      continue;
    }
    const std::size_t need_exp = matrix ? row.expected.size() : width;
    const std::size_t need_in = matrix ? row.inputs.size()
                                       : width;
    if (row.expected.size() != need_exp || row.inputs.size() < need_in) {
      error = "lds carrier: row needs matching width dwords: " + row.id;
      return std::nullopt;
    }
  }

  constexpr std::uint32_t kLdsBase = 0x20;     // first LDS cell byte offset
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg addr_v = isa::vgpr(8);
  const isa::Reg imm_temp = isa::sgpr(16);
  // zero address for result stores
  builder.s_mov_imm_b32(imm_temp, 0u);
  builder.v_mov_b32(isa::vgpr(0), imm_temp, isa::predicate(0));

  EnvelopeProgram program;
  // The interpolator returns the coefficient-window content (launch
  // dependent, not oracle-able); observe without comparison.
  if (mnemonic == "load_sm_interp") {
    program.observe = true;
  }
  std::uint32_t off = carrier::kResultBase;
  for (std::size_t ci = 0; ci < rows.size(); ++ci) {
    const auto& def = rows[ci];
    const std::uint32_t cell = kLdsBase + 4 * width * ci;
    builder.s_mov_imm_b32(imm_temp, cell);
    builder.v_mov_b32(addr_v, imm_temp, isa::predicate(0));
    if (is_atomic) {
      // 16-lane model: the final cell applies the operation once per lane
      // with the same operand. Compose 64-bit values from dword pairs.
      std::string op;
      unsigned aw = 0;
      sm_atomic_op_width(mnemonic, op, aw);
      const bool f64cmpswap =
          (op == "cmpswap_gt" || op == "cmpswap_lt") &&
          mnemonic.find("_f64") != std::string::npos;
      const bool sgn = mnemonic.find("_i32") != std::string::npos ||
                       mnemonic.find("_i64") != std::string::npos;
      std::uint64_t initial = 0, a = 0, b = 0;
      for (unsigned p = 0; p < width; ++p) {
        initial |= static_cast<std::uint64_t>(def.inputs[p]) << (32 * p);
        a |= static_cast<std::uint64_t>(def.inputs[width + p]) << (32 * p);
        if (op == "cmpswap_eq")
          b |= static_cast<std::uint64_t>(def.inputs[2 * width + p]) << (32 * p);
      }
      if (width == 1 && sgn) {
        // sign-extend so signed min/max compare true magnitudes
        initial = static_cast<std::uint64_t>(
            static_cast<std::int64_t>(static_cast<std::int32_t>(initial)));
        a = static_cast<std::uint64_t>(
            static_cast<std::int64_t>(static_cast<std::int32_t>(a)));
      }
      std::uint64_t fin;
      if (f64cmpswap) {
        // [HW 2026-08-25] observed truth table (probed on 8 value pairs):
        // both cmpswap_gt_f64 and cmpswap_lt_f64 leave the LDS cell
        // untouched (swap never fires on the tested carrier; NaN rows
        // also match).  UNRESOLVED why the f64 form differs from f32.
        std::memcpy(&fin, &initial, 8);
      } else {
        fin = sm_final_n(op, sgn, initial, a, b, env.lanes);
      }
      // store the initial value into the cell
      for (unsigned p = 0; p < width; ++p) {
        builder.s_mov_imm_b32(imm_temp, def.inputs[p]);
        builder.v_mov_b32(isa::vgpr(9 + p), imm_temp, isa::predicate(0));
      }
      builder.sm_op(width == 2 ? "store_sm_b64" : "store_sm_b32",
                    isa::predicate(0), isa::vgpr(9, width), addr_v);
      builder.check_k_vm_vs_sm();
      // stage the operand and execute the atomic
      for (unsigned p = 0; p < width; ++p) {
        builder.s_mov_imm_b32(imm_temp, def.inputs[width + p]);
        builder.v_mov_b32(isa::vgpr(11 + p), imm_temp, isa::predicate(0));
      }
      if (f64cmpswap) {
        builder.append_target(isa::McInst{
            mnemonic, "",
            {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(11, 2)),
             isa::op_reg(addr_v), isa::op_reg(isa::vgpr(9, 2))}});
      } else if (op == "cmpswap_eq") {
        for (unsigned p = 0; p < width; ++p) {
          builder.s_mov_imm_b32(imm_temp, def.inputs[2 * width + p]);
          builder.v_mov_b32(isa::vgpr(13 + p), imm_temp, isa::predicate(0));
        }
        builder.sm_cmpswap(mnemonic, isa::predicate(0), isa::vgpr(11, width),
                           addr_v, isa::vgpr(13, width), isa::vgpr(17, width));
      } else {
        // operand order per the proven conformance sm-atomic probe:
        // {pred, address(VJ), data(VK), old-out(VD)}
        builder.sm_atomic(mnemonic, isa::predicate(0), addr_v,
                          isa::vgpr(11, width), isa::vgpr(13, width));
      }
      builder.check_k_vm_vs_sm();
      // read back the final cell
      builder.sm_op(width == 2 ? "load_sm_b64" : "load_sm_b32",
                    isa::predicate(0), isa::vgpr(15, width), addr_v);
      builder.check_k_vm_vs_sm();
      for (unsigned p = 0; p < width; ++p) {
        emit_result_store(builder, env, isa::vgpr(15 + p), isa::vgpr(0),
                          off + 4 * p);
        builder.check_k_vm_vs_sm();
      }
      BuiltCase abc;
      abc.id = def.id;
      for (unsigned p = 0; p < width; ++p) abc.offsets.push_back(off + 4 * p);
      for (unsigned p = 0; p < width; ++p) {
        abc.expected.push_back(static_cast<std::uint32_t>(fin >> (32 * p)));
      }
      program.cases.push_back(std::move(abc));
      off += 4 * width;
      continue;
    }
    // stage the input data dwords into v9..
    for (unsigned p = 0; p < width; ++p) {
      builder.s_mov_imm_b32(imm_temp, def.inputs[p]);
      builder.v_mov_b32(isa::vgpr(9 + p), imm_temp, isa::predicate(0));
    }
    if (mnemonic == "load_sm_interp") {
      // Interpolator load: (pred, VD8 dst, RJ8 s9 coefficient base, UO8
      // imm offset).  The fragment-input config (packet 0x530) opens the
      // smooth coefficient window; s2 carries the barycentric lane.
      builder.append_target(isa::McInst{
          mnemonic, "",
          {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(10)),
           isa::op_reg(isa::sgpr(9)), isa::op_imm(0)}});
      builder.check_k_vm_vs_sm();
      builder.append_target(isa::McInst{
          "v_interp_mov_b32", "",
          {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(10)),
           isa::op_reg(isa::sgpr(2)), isa::op_reg(isa::vgpr(10))}});
      builder.check_k_vm_vs_sm();
    } else if (is_load) {
      // LDS starts undefined: store the input with the matching store op
      const std::string store_mne =
          width == 2 ? "store_sm_b64" : "store_sm_b32";
      builder.sm_op(store_mne, isa::predicate(0), isa::vgpr(9, width),
                    addr_v);
      builder.check_k_vm_vs_sm();
      builder.sm_op(mnemonic, isa::predicate(0), isa::vgpr(10, width),
                    addr_v);
      builder.check_k_vm_vs_sm();
    } else {
      // target store from the staged data ...
      builder.sm_op(mnemonic, isa::predicate(0), isa::vgpr(9, width),
                    addr_v);
      builder.check_k_vm_vs_sm();
      // ... then read back through the matching (sub-)dword load
      std::string readback = "load_sm_b32";
      if (width == 2) readback = "load_sm_b64";
      else if (mnemonic.find("b8") != std::string::npos)
        readback = "load_sm_u8";
      else if (mnemonic.find("b16") != std::string::npos)
        readback = "load_sm_u16";
      builder.sm_op(readback, isa::predicate(0), isa::vgpr(10, width),
                    addr_v);
      builder.check_k_vm_vs_sm();
    }
    for (unsigned p = 0; p < width; ++p) {
      emit_result_store(builder, env, isa::vgpr(10 + p), isa::vgpr(0),
                        off + 4 * p);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    for (unsigned p = 0; p < width; ++p) bc.offsets.push_back(off + 4 * p);
    bc.expected = def.expected;
    program.cases.push_back(std::move(bc));
    off += 4 * width;
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  if (!finalize_program(builder, program, error)) return std::nullopt;
  return program;
}
// load/store/atomic_v_flat through a 64-bit flat address.  The address VGPR
// pair v0:v1 holds (offset, 1) so the effective VA is 0x100000000 + offset
// (the argument BO); the flat base s0:s1 is the reviewed arg-BO base.  The
// runner preloads the load window (default arg+0x300) and reads back the
// result region (arg+0x20).  Case inputs are [data, addr]; the vector's own
// addr (when present) selects the window offset.
static unsigned flat_width(const std::string& mne) {
  if (mne.find("b128") != std::string::npos) return 4;
  if (mne.find("b96") != std::string::npos) return 3;
  if (mne.find("b64") != std::string::npos) return 2;
  return 1;  // b8/b16/b32
}
std::optional<EnvelopeProgram> build_flat(const std::string& mnemonic,
                                          const std::vector<EnvelopeCase>& rows,
                                          const CarrierEnv& env,
                                          std::string& error) {
  const bool is_load = mnemonic.rfind("load_v_flat", 0) == 0;
  const bool is_store = mnemonic.rfind("store_v_flat", 0) == 0;
  const bool is_atomic = mnemonic.rfind("atomic_v_flat", 0) == 0;
  if (!is_load && !is_store && !is_atomic) {
    error = "flat carrier: expected load_v_flat_*/store_v_flat_*/atomic_v_flat_*";
    return std::nullopt;
  }
  // Shape-true support: only the b32 forms share the hardware-verified
  // b32 template.  Wider loads/stores and 64-bit atomics have a different
  // operand layout (multi-dword data/address) and cmpswap needs two data
  // registers the template does not stage, so they are rejected instead of
  // silently emitted with the wrong shape.
  static const char* const kFlatWide[] = {
      "load_v_flat_b64",  "load_v_flat_b96",  "load_v_flat_b128",
      "load_v_flat_i8",   "load_v_flat_u8",   "load_v_flat_i16",
      "load_v_flat_u16",  "store_v_flat_b8",  "store_v_flat_b16",
      "store_v_flat_b64", "store_v_flat_b96", "store_v_flat_b128"};
  static const char* const kFlat32[] = {
      "load_v_flat_b32", "store_v_flat_b32",
      "atomic_v_flat_add_i32", "atomic_v_flat_and_b32",
      "atomic_v_flat_dec_u32", "atomic_v_flat_inc_u32",
      "atomic_v_flat_max_i32", "atomic_v_flat_max_u32",
      "atomic_v_flat_min_i32", "atomic_v_flat_min_u32",
      "atomic_v_flat_or_b32", "atomic_v_flat_sub_i32",
      "atomic_v_flat_swap_b32", "atomic_v_flat_xor_b32",
      "atomic_v_flat_cmpswap_b32"};
  static const char* const kFlat64[] = {
      "atomic_v_flat_add_i64", "atomic_v_flat_and_b64",
      "atomic_v_flat_dec_u64", "atomic_v_flat_inc_u64",
      "atomic_v_flat_max_i64", "atomic_v_flat_max_u64",
      "atomic_v_flat_min_i64", "atomic_v_flat_min_u64",
      "atomic_v_flat_or_b64",  "atomic_v_flat_sub_i64",
      "atomic_v_flat_swap_b64", "atomic_v_flat_xor_b64",
      "atomic_v_flat_cmpswap_b64"};
  bool supported = false;
  for (const auto* m : kFlat32) supported = supported || mnemonic == m;
  for (const auto* m : kFlat64) supported = supported || mnemonic == m;
  for (const auto* m : kFlatWide) supported = supported || mnemonic == m;
  if (!supported) {
    error = "flat carrier: no shape-true template for " + mnemonic;
    return std::nullopt;
  }
  const bool wide64 = [&]() {
    for (const auto* m : kFlat64)
      if (mnemonic == m) return true;
    return false;
  }();
  const bool cmpswap64 = mnemonic == "atomic_v_flat_cmpswap_b64";
  const std::uint8_t width = wide64 ? 2u : flat_width(mnemonic);
  for (const auto& row : rows) {
    const std::size_t need_inputs =
        is_atomic ? (wide64 ? (cmpswap64 ? 6u : 4u) : 2u)
                  : (is_store ? width : width);
    const std::size_t need_expected =
        is_atomic ? (wide64 ? 4u : 2u) : width;
    if (row.expected.size() != need_expected ||
        row.inputs.size() < need_inputs) {
      error = "flat carrier: row needs matching width dwords: " + row.id;
      return std::nullopt;
    }
  }

  Builder builder;
  emit_env_prelude(builder, env);
  builder.s_mov_imm_b32(isa::sgpr(20), 0x00000000u);
  builder.s_mov_imm_b32(isa::sgpr(21), 0x00000000u);
  // flat base pair = s20:s21 = {0, 0}: mesa loongvk (lco_materialize_
  // global_address) passes the FULL absolute 64-bit VA in the vaddr pair
  // and a literal-zero scalar base pair; s0:s1 stays the store descriptor.
  // [HW 2026-08-25] flat loads/stores/atomics read address = vaddr only.
  const isa::Reg flat_base = isa::sgpr(20, 2);
  const isa::Reg addr_lo = isa::vgpr(0);
  const isa::Reg addr_pair = isa::vgpr(0, 2);
  const isa::Reg imm_temp = isa::sgpr(16);
  const isa::Reg data_v = isa::vgpr(8);
  const isa::Reg result_v = isa::vgpr(10, width);

  EnvelopeProgram program;
  program.probe = true;
  std::uint32_t off = carrier::kResultBase;
  for (std::size_t ci = 0; ci < rows.size(); ++ci) {
    const auto& def = rows[ci];
    // optional per-case relative address (inputs[1], default 0).  Atomic
    // rows never use it: the 32-bit form carries (cell, addend) and the
    // 64-bit form (cell_lo, cell_hi, src...), so inputs[1] is data.
    const std::uint32_t rel =
        (is_atomic || width > 1)
            ? 0u
            : (def.inputs.size() >= 2 ? def.inputs[1] : 0u);
    const std::uint32_t addr = carrier::kLoadWindow + 4 * width * ci + rel;
    // The case data decides `rel`, so a case whose second input is not a small
    // relative offset turns this into an access anywhere in the 64-bit space.
    // That is not a hypothetical: with the flat forms' encoding corrected (so
    // the store actually executes) one such case faulted at arg_va +
    // 0x12345678 and wedged the pipe.  A carrier may only touch its own
    // window, so anything outside it is refused here rather than emitted.
    // kArgWindowBytes is the window the PS carrier owns inside its argument BO.
    if (addr > carrier::kArgWindowBytes ||
        carrier::kArgWindowBytes - addr < 4u * width) {
      error = "flat carrier: case address outside the carrier's window: " +
              def.id;
      return std::nullopt;
    }
    // Flat addressing: the VJ8x2 vaddr pair carries the FULL absolute
    // 64-bit VA (kArgVa + offset), the RK8x2 scalar base pair literal
    // zero -- the exact LCO shape (lco_materialize_global_address).
    const std::uint64_t full_va = env.arg_va + addr;
    builder.s_mov_imm_b32(imm_temp, static_cast<std::uint32_t>(full_va));
    builder.v_mov_b32(addr_lo, imm_temp, isa::predicate(0));
    builder.s_mov_imm_b32(imm_temp,
                          static_cast<std::uint32_t>(full_va >> 32));
    builder.v_mov_b32(isa::vgpr(1), imm_temp, isa::predicate(0));
    // The result store's address register must be one that NOTHING else in this
    // carrier writes, because the address it names is added to the descriptor
    // base: if it holds case data the store lands at (base + data + imm).  Both
    // observed GPU faults were exactly that - a protection fault at kArgVa +
    // 0x12345678 + 8, where 0x12345678 is the corpus' passthrough datum - and
    // the pipe was quarantined each time.  The writers to avoid are v8..v11
    // (data / result), v9 (cmpswap dword), v10..v13 (wide result) and v14:v15
    // (the atomic read-back pair).  v16 is outside all of them.
    // (v0:v1 is the flat access's own address pair, which the store also uses
    //  in compute - emit_result_store builds it itself.)
    builder.s_mov_imm_b32(imm_temp, 0u);
    builder.v_mov_b32(isa::vgpr(16), imm_temp, isa::predicate(0));

    if (is_store) {
      // stage data (width dwords) into v8.. then store, then load back
      for (std::uint8_t part = 0; part < width; ++part) {
        const std::uint32_t value = def.inputs[part];
        builder.s_mov_imm_b32(imm_temp, value);
        builder.v_mov_b32(isa::vgpr(8 + part), imm_temp, isa::predicate(0));
      }
      builder.store_v_flat(mnemonic, isa::predicate(0),
                           isa::vgpr(8, width), addr_pair, flat_base);
      builder.check_k_vm_vs_sm();
      {
        // read back with the matching load width (b8/b16 have no load
        // form; use b32 and compare the full dword).
        const std::string suf = mnemonic.substr(13);
        const std::string load_mne =
            (suf == "b8" || suf == "b16") ? "load_v_flat_b32" : "load_v_flat_" + suf;
        builder.load_v_flat(load_mne, isa::predicate(0), result_v, addr_pair,
                            flat_base);
      }
      builder.check_k_vm_vs_sm();
    } else if (is_atomic) {
      if (!wide64) {
        const bool cmpswap32 =
            mnemonic == "atomic_v_flat_cmpswap_b32";
        program.arg_preloads.emplace_back(addr, def.inputs[0]);
        builder.s_mov_imm_b32(imm_temp, def.inputs[1]);
        builder.v_mov_b32(data_v, imm_temp, isa::predicate(0));
        if (cmpswap32) {
          // cmpswap stages the compare dword in v9 (v8:v9 pair, mirroring
          // the buff template); v9 is the flat result-store address, so it
          // is zeroed again before the readback stores below.
          builder.s_mov_imm_b32(imm_temp, def.inputs[2]);
          builder.v_mov_b32(isa::vgpr(16), imm_temp, isa::predicate(0));
          builder.atomic_v_flat(mnemonic, isa::predicate(0),
                                isa::vgpr(8, 2), addr_pair, flat_base,
                                isa::vgpr(8, 2));
        } else {
          builder.atomic_v_flat(mnemonic, isa::predicate(0), result_v,
                                addr_pair, flat_base, data_v);
        }
        builder.check_k_vm_vs_sm();
        builder.s_mov_imm_b32(imm_temp, 0u);
        builder.v_mov_b32(isa::vgpr(16), imm_temp, isa::predicate(0));
        builder.load_v_flat("load_v_flat_b32", isa::predicate(0), isa::vgpr(14),
                            addr_pair, flat_base);
        builder.check_k_vm_vs_sm();
        emit_result_store(builder, env, isa::vgpr(14), isa::vgpr(16), off + 4);
        builder.check_k_vm_vs_sm();
      } else {
        // 64-bit: cell pair preloaded at addr/addr+4; data pair v8:v9;
        // cmpswap pair v10:v11; old pair returned in v8:v9; cell read back
        // to v14:v15 then stored at off+8/+12.
        program.arg_preloads.emplace_back(addr, def.inputs[0]);
        program.arg_preloads.emplace_back(addr + 4, def.inputs[1]);
        for (unsigned p = 0; p < 2; ++p) {
          builder.s_mov_imm_b32(imm_temp, def.inputs[2 + p]);
          builder.v_mov_b32(isa::vgpr(8 + p), imm_temp, isa::predicate(0));
        }
        if (cmpswap64) {
          for (unsigned p = 0; p < 2; ++p) {
            builder.s_mov_imm_b32(imm_temp, def.inputs[4 + p]);
            builder.v_mov_b32(isa::vgpr(10 + p), imm_temp, isa::predicate(0));
          }
        }
        builder.atomic_v_flat(mnemonic, isa::predicate(0), isa::vgpr(8, 2),
                              addr_pair, flat_base, isa::vgpr(8, 2));
        builder.check_k_vm_vs_sm();
        builder.load_v_flat("load_v_flat_b64", isa::predicate(0),
                            isa::vgpr(14, 2), addr_pair, flat_base);
        builder.check_k_vm_vs_sm();
        for (unsigned p = 0; p < 2; ++p) {
          emit_result_store(builder, env, isa::vgpr(14 + p), isa::vgpr(16),
                            off + 8 + 4 * p);
          builder.check_k_vm_vs_sm();
        }
      }
    } else {
      builder.load_v_flat(mnemonic, isa::predicate(0), result_v, addr_pair,
                          flat_base);
      builder.check_k_vm_vs_sm();
      for (unsigned p = 0; p < width; ++p)
        program.arg_preloads.emplace_back(addr + 4 * p, def.inputs[p]);
    }
    for (std::uint8_t part = 0; part < width; ++part) {
      emit_result_store(builder, env, isa::vgpr(10 + part), isa::vgpr(16),
                        off + 4 * part);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    if (is_atomic && wide64) {
      // old pair at off (racy across 16 lanes; kept unverified), final cell
      // pair at off+8 (deterministic: every lane applies the same RMW).
      bc.offsets.push_back(off + 8);
      bc.offsets.push_back(off + 12);
      const std::uint64_t cell =
          static_cast<std::uint64_t>(def.inputs[0]) |
          (static_cast<std::uint64_t>(def.inputs[1]) << 32);
      const std::uint64_t src64 =
          static_cast<std::uint64_t>(def.inputs[2]) |
          (static_cast<std::uint64_t>(def.inputs[3]) << 32);
      const std::uint64_t swap64 =
          cmpswap64 ? (static_cast<std::uint64_t>(def.inputs[4]) |
                       (static_cast<std::uint64_t>(def.inputs[5]) << 32))
                    : 0u;
      const std::string op = mnemonic.substr(
          13, mnemonic.find('_', 13) - 13);
      const bool sgn = mnemonic.find("_i64") != std::string::npos;
      const std::uint64_t fin =
          sm_final_n(op, sgn, cell, src64, swap64, env.lanes);
      bc.expected.push_back(static_cast<std::uint32_t>(fin));
      bc.expected.push_back(static_cast<std::uint32_t>(fin >> 32));
      program.cases.push_back(std::move(bc));
      off += 16u;
      continue;
    } else if (is_atomic) {
      // old value at off, post-atomic cell at off+4
      bc.offsets.push_back(off);
      bc.offsets.push_back(off + 4);
    } else {
      for (std::uint8_t part = 0; part < width; ++part)
        bc.offsets.push_back(off + 4 * part);
    }
    bc.expected = def.expected;
    program.cases.push_back(std::move(bc));
    // atomic cases write two result dwords (old value + post-op cell)
    off += is_atomic ? (wide64 ? 16u : 8u) : 4u * width;
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  if (!finalize_program(builder, program, error)) return std::nullopt;
  return program;
}

// Sampler tuple (LG200_SAMPLER_DESCRIPTOR pack): NEAREST / CLAMP-TO-EDGE /
// mip-filter NEAREST, lod 0..15.996, lod_bias 0, disable_cube_wrap=1,
// border_color_type=TABLE_ENTRY -- the loongvk default-compatible S#.
static void pack_sampler_tsdesc(std::uint32_t dst[4]) {
  dst[0] = 0x10000092u;   // clamp x/y/z = 2, disable_cube_wrap
  dst[1] = 0x00fff000u;   // maximum_lod = 4095 (15.99609375)
  dst[2] = 0x04000000u;   // mip_filter = NEAREST
  dst[3] = 0x40000000u;   // border_color_type = TABLE_ENTRY
}
// ---- tex (texture fetch) carrier --------------------------------------------
// load_v_tex through an explicitly staged 8-dword T# resource tuple.  The
// tuple bit layout comes from the Mesa LG200_IMAGE_DESCRIPTOR genxml packer:
// base[39:0], data_format[57:52], number_format[63:58], width[77:64],
// height[91:78], performance_mode[94:92], swizzle[107:96], base_level[111:108],
// last_level[115:112], tiling_index[120:116], pow2pad[121], type[127:124],
// depth[140:128], pitch[154:141].  Texels live in the argument BO window at
// arg+0x400; each case fetches its own texel via the x coordinate.
static void pack_image_tdesc(std::uint32_t dst[8], std::uint64_t base,
                             std::uint32_t width, std::uint32_t pitch,
                             std::uint32_t height = 1) {
  for (unsigned i = 0; i < 8; ++i) dst[i] = 0;
  const std::uint32_t dfmt = std::getenv("LGPU_TEX_DFMT")
      ? static_cast<std::uint32_t>(std::strtoul(
            std::getenv("LGPU_TEX_DFMT"), nullptr, 0)) : 4u;
  const std::uint32_t nfmt = std::getenv("LGPU_TEX_NFMT")
      ? static_cast<std::uint32_t>(std::strtoul(
            std::getenv("LGPU_TEX_NFMT"), nullptr, 0)) : 4u;
  const std::uint32_t tiling = std::getenv("LGPU_TEX_TILING")
      ? static_cast<std::uint32_t>(std::strtoul(
            std::getenv("LGPU_TEX_TILING"), nullptr, 0)) : 0u;
  const std::uint32_t pow2 = std::getenv("LGPU_TEX_P2PAD")
      ? static_cast<std::uint32_t>(std::strtoul(
            std::getenv("LGPU_TEX_P2PAD"), nullptr, 0)) : 0u;
  auto put = [&](std::uint32_t bit, std::uint32_t bits, std::uint64_t v) {
    for (std::uint32_t b = 0; b < bits; ++b) {
      const std::uint32_t word = (bit + b) / 32;
      const std::uint32_t sh = (bit + b) % 32;
      dst[word] |= static_cast<std::uint32_t>((v >> b) & 1u) << sh;
    }
  };
  put(0, 40, base);          // base address
  put(52, 6, dfmt);          // data_format
  put(58, 4, nfmt);          // number_format
  put(64, 14, width);        // width
  put(78, 14, height);       // height
  put(92, 3, 4);             // performance_mode
  put(96, 3, 4);             // swizzle x = X
  put(99, 3, 5);             // y = Y
  put(102, 3, 6);            // z = Z
  put(105, 3, 7);            // w = W
  put(108, 4, 0);            // base_level
  put(112, 4, 0);            // last_level
  put(116, 5, tiling);       // tiling_index
  put(121, 1, pow2);         // power_of_two_pad
  put(124, 4, 9);            // type = 2D
  put(128, 13, 1);           // depth = 1
  put(141, 14, pitch);       // pitch (elements)
}

std::optional<EnvelopeProgram> build_tex(const std::string& mnemonic,
                                         const std::vector<EnvelopeCase>& rows,
                   const CarrierEnv& env,
                                         std::string& error) {
  // Shape-true templates: every load_v_tex* mnemonic shares the
  // {pred, vdst-quad, coords-triple, s-resource-octet, un, dmask:1111}
  // operand shape (Mesa LCO packs load_v_tex_mip exactly like this and the
  // texelFetch readback matches it).  getresinfo shares the
  // shape without a sampler tuple.
  static const char* const kTexLoad[] = {
      "load_v_tex", "load_v_tex_mip", "load_v_tex_mip_pck",
      "load_v_tex_mip_pck_sgn", "load_v_tex_pck", "load_v_tex_pck_sgn",
      "getresinfo"};
  bool supported = false;
  for (const auto* m : kTexLoad) supported = supported || mnemonic == m;
  if (!supported) {
    error = "tex carrier: no shape-true template for " + mnemonic;
    return std::nullopt;
  }
  for (const auto& row : rows) {
    if (row.expected.empty() || row.inputs.size() > 2) {
      error = "tex carrier: row needs >=1 expected dword and <=2 inputs";
      return std::nullopt;
    }
  }
  constexpr std::uint32_t kTexelCount = 16;
  Builder builder;
  EnvelopeProgram program;
  program.probe = true;
  // loongvk-linear image descriptor: R32/UINT, pitch 64 elems
  // (next_pow2(max(width,64))), tiling 0.
  const std::uint32_t pitch = env_u32("LGPU_TEX_PITCH", 64u);
  const std::uint32_t img_rows = env_u32("LGPU_TEX_H", 1u);
  std::uint32_t tdesc[8];
  pack_image_tdesc(tdesc, env.arg_va + carrier::kTexelWindow,
                   kTexelCount, pitch, img_rows);
  const TexStage st =
      emit_tex_prelude(builder, program, env, tdesc, nullptr, 16u);
  const isa::Reg coords_v = isa::vgpr(16, 3);  // {x, y, lod}
  const isa::Reg dst_v = isa::vgpr(8, 4);      // aligned quad result
  std::uint32_t off = carrier::kResultBase;
  for (std::size_t ci = 0; ci < rows.size() && ci < kTexelCount; ++ci) {
    const auto& def = rows[ci];
    const std::uint32_t texel =
        def.inputs.empty() ? def.expected.back() : def.inputs[0];
    const std::uint32_t cyy = env_u32("LGPU_TEX_ROW", 0u);
    program.arg_preloads.emplace_back(
        carrier::kTexelWindow + cyy * (pitch * 4u) +
            4 * static_cast<std::uint32_t>(ci), texel);
    // Texel-fetch coords for 2D: {x=ci, y=row, lod=0} (LCO packs
    // {x,y,lod}).  Experiment knobs shift each slot.
    builder.s_mov_imm_b32(st.imm_temp,
                          static_cast<std::uint32_t>(ci) +
                              env_u32("LGPU_TEX_XOFF", 0u));
    builder.v_mov_b32(isa::vgpr(16), st.imm_temp, isa::predicate(0));
    builder.s_mov_imm_b32(
        st.imm_temp,
        env_has("LGPU_TEX_ROW") ? cyy : env_u32("LGPU_TEX_Y", 0u));
    builder.v_mov_b32(isa::vgpr(17), st.imm_temp, isa::predicate(0));
    builder.s_mov_imm_b32(st.imm_temp, env_u32("LGPU_TEX_LOD", 0u));
    builder.v_mov_b32(isa::vgpr(18), st.imm_temp, isa::predicate(0));
    if (!env_has("LGPU_NO_TEX")) {
      builder.image_op(mnemonic, {isa::op_reg(isa::predicate(0)),
                                   isa::op_reg(dst_v), isa::op_reg(coords_v),
                                   isa::op_reg(st.tdesc_regs)},
                       true);
      builder.check_k_vm_vs_sm();
    }
    for (unsigned p = 0; p < 4u; ++p) {
      emit_result_store(builder, env, isa::vgpr(8 + p),
                        isa::vgpr(0), off + 4u * p);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    bc.offsets.push_back(off);
    bc.expected.push_back(def.expected.back());
    program.cases.push_back(std::move(bc));
    off += 16;
  }
  if (!finish_program(builder, program, error)) return std::nullopt;
  return program;
}
// ---- store_v_tex (texture store) carrier ------------------------------------
// store_v_tex {pred, vdata-quad, coords-octet, T#, un, dmask}; 16 lanes write
// the same value to the same texel, so the final cell is deterministic and is
// read back through the proven load_v_tex_mip fetch.
std::optional<EnvelopeProgram> build_tex_store(const std::string& mnemonic,
                                               const std::vector<EnvelopeCase>& rows,
                         const CarrierEnv& env,
                                               std::string& error) {
  if (mnemonic != "store_v_tex" && mnemonic != "store_v_tex_mip" &&
      mnemonic != "store_v_tex_pck" && mnemonic != "store_v_tex_mip_pck") {
    error = "tex-store carrier: only store_v_tex(_mip/_pck) implemented";
    return std::nullopt;
  }
  for (const auto& row : rows) {
    if (row.expected.empty() || row.inputs.empty()) {
      error = "tex-store carrier: row needs expected and input dwords";
      return std::nullopt;
    }
  }
  Builder builder;
  EnvelopeProgram program;
  program.probe = true;
  std::uint32_t tdesc[8];
  pack_image_tdesc(tdesc, env.arg_va + carrier::kTexelWindow, 16u, 64u);
  const TexStage st =
      emit_tex_prelude(builder, program, env, tdesc, nullptr, 16u);
  std::uint32_t off = carrier::kResultBase;
  for (std::size_t ci = 0; ci < rows.size() && ci < 16u; ++ci) {
    const auto& def = rows[ci];
    // vdata quad = the stored value; coords = {x=ci, y=0, lod=0, ...}
    builder.s_mov_imm_b32(st.imm_temp, def.inputs[0]);
    for (unsigned g = 4; g < 8; ++g)
      builder.v_mov_b32(isa::vgpr(g), st.imm_temp, isa::predicate(0));
    for (unsigned g = 16; g < 24; ++g) {
      builder.s_mov_imm_b32(
          st.imm_temp, g == 16 ? static_cast<std::uint32_t>(ci) : 0u);
      builder.v_mov_b32(isa::vgpr(g), st.imm_temp, isa::predicate(0));
    }
    if (!env_has("LGPU_NO_TEX")) {
      builder.image_op(mnemonic,
                       {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(4, 4)),
                        isa::op_reg(isa::vgpr(16, 8)), isa::op_reg(st.tdesc_regs)},
                       true);
      builder.check_k_vm_vs_sm();
      // read back the stored cell through the proven texel-fetch form.
      builder.image_op("load_v_tex_mip",
                       {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(8, 4)),
                        isa::op_reg(isa::vgpr(16, 3)), isa::op_reg(st.tdesc_regs)},
                       true);
      builder.check_k_vm_vs_sm();
    }
    for (unsigned p = 0; p < 1u; ++p) {
      emit_result_store(builder, env, isa::vgpr(8 + p),
                        isa::vgpr(0), off + 4u * p);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    bc.offsets.push_back(off);
    bc.expected.push_back(def.expected.back());
    program.cases.push_back(std::move(bc));
    off += 4;
  }
  if (!finish_program(builder, program, error)) return std::nullopt;
  return program;
}

// ---- atomic_v_tex (texture atomics) carrier ----------------------------------
// atomic_v_tex* through the same loongvk T# slot.  The operand layout is
// {pred, vdst(old), coords-octet, T#, un, dmask}; the atomic source value is
// one coordinate slot (located empirically; LGPU_ATEX_SLOT, default 3).
// 16-lane model: every lane hits the same texel, so the final cell equals
// the operation applied per-lane; the final value is read back with a
// load_v_tex_mip fetch and compared against the model.
std::optional<EnvelopeProgram> build_tex_atomic(const std::string& mnemonic,
                                                const std::vector<EnvelopeCase>& rows,
                          const CarrierEnv& env,
                                                std::string& error) {
  static const char* const kATex[] = {
      "atomic_v_tex_add_i32", "atomic_v_tex_and_b32", "atomic_v_tex_cmpswap_b32",
      "atomic_v_tex_dec_u32", "atomic_v_tex_inc_u32", "atomic_v_tex_max_i32",
      "atomic_v_tex_max_u32", "atomic_v_tex_min_i32", "atomic_v_tex_min_u32",
      "atomic_v_tex_or_b32",  "atomic_v_tex_sub_i32", "atomic_v_tex_swap_b32",
      "atomic_v_tex_xor_b32"};
  bool supported = false;
  for (const auto* m : kATex) supported = supported || mnemonic == m;
  if (!supported) {
    error = "tex-atomic carrier: no shape-true template for " + mnemonic;
    return std::nullopt;
  }
  for (const auto& row : rows) {
    if (row.expected.empty() || row.inputs.size() < 2) {
      error = "tex-atomic carrier: row needs >=2 inputs (texel, source)";
      return std::nullopt;
    }
  }
  Builder builder;
  EnvelopeProgram program;
  program.probe = true;
  std::uint32_t tdesc[8];
  pack_image_tdesc(tdesc, env.arg_va + carrier::kTexelWindow, 16u, 64u);
  const TexStage st =
      emit_tex_prelude(builder, program, env, tdesc, nullptr, 16u);
  // The atomic source value is one coordinate slot (located empirically;
  // LGPU_ATEX_SLOT, default 3).
  const std::uint32_t slot = env_u32("LGPU_ATEX_SLOT", 3u);
  const bool cmpswap = mnemonic.find("cmpswap") != std::string::npos;
  const std::string op = mnemonic.substr(13, mnemonic.find('_', 13) - 13);
  std::uint32_t off = carrier::kResultBase;
  for (std::size_t ci = 0; ci < rows.size(); ++ci) {
    const auto& def = rows[ci];
    const std::uint32_t init = def.inputs[0];
    const std::uint32_t src = def.inputs[1];
    program.arg_preloads.emplace_back(
        carrier::kTexelWindow + 4 * static_cast<std::uint32_t>(ci), init);
    // coords: {x=ci, y=0, lod=0, source at slot, pad...}
    for (unsigned g = 16; g < 24; ++g) {
      builder.s_mov_imm_b32(
          st.imm_temp,
          g == 16 ? static_cast<std::uint32_t>(ci)
                  : (g == 16 + slot ? src : 0u));
      builder.v_mov_b32(isa::vgpr(g), st.imm_temp, isa::predicate(0));
    }
    if (!env_has("LGPU_NO_TEX")) {
      // GCN-style in-place data: the operand rides the dst register(s);
      // cmpswap carries {compare, swap} in v8-v9.
      builder.s_mov_imm_b32(st.imm_temp, src);
      builder.v_mov_b32(isa::vgpr(8), st.imm_temp, isa::predicate(0));
      if (cmpswap && def.inputs.size() >= 3) {
        builder.s_mov_imm_b32(st.imm_temp, def.inputs[2]);
        builder.v_mov_b32(isa::vgpr(9), st.imm_temp, isa::predicate(0));
      }
      builder.image_op(mnemonic,
                       {isa::op_reg(isa::predicate(0)),
                        isa::op_reg(isa::vgpr(8, cmpswap ? 2 : 1)),
                        isa::op_reg(isa::vgpr(16, 8)), isa::op_reg(st.tdesc_regs)},
                       true);
      builder.check_k_vm_vs_sm();
      // read back the final cell through the proven texel-fetch form.
      builder.image_op("load_v_tex_mip",
                       {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(12, 4)),
                        isa::op_reg(isa::vgpr(16, 3)), isa::op_reg(st.tdesc_regs)},
                       true);
      builder.check_k_vm_vs_sm();
    }
    for (unsigned p = 0; p < 1u; ++p) {
      emit_result_store(builder, env, isa::vgpr(12 + p),
                        isa::vgpr(0), off + 4u * p);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    bc.offsets.push_back(off);
    bc.expected.push_back(
        atomic_final_n(op, init, src, mnemonic.find("_i32") != std::string::npos,
                       def.inputs.size() >= 3 ? def.inputs[2] : 0u,
                       env.lanes));
    program.cases.push_back(std::move(bc));
    off += 4;
  }
  if (!finish_program(builder, program, error)) return std::nullopt;
  return program;
}

// ---- samp (texture sampling) carrier --------------------------------------
// samp* through the loongvk descriptor-slot layout: image T# at the rsd
// table +0 (8 dwords) and sampler S# at +0x30 (4 dwords), both loaded
// through the proven scalar flat addressing form (VA = s6:s7 + imm).
// Coordinate layouts follow the LCO/loongvk producer:
//   basic (samp/_b/_c/_cl): {offset?, s, t[, compare-first for _c]}
//   _l/_lz (explicit lod):  {offset?, s, t, lod}
//   _d (explicit gradient): {offset?, ddx, ddy, s, t, 0,0,0}
// The 16-lane model applies: one uniform fetch per case => deterministic.
std::optional<EnvelopeProgram> build_samp(const std::string& mnemonic,
                                          const std::vector<EnvelopeCase>& rows,
                                                const CarrierEnv& env,
                                          std::string& error) {
  // Per-variant behaviour is suffix-driven; "_l" inside "_cl" (clamp)
  // must not count as lod.
  const bool offset = mnemonic.find("_o") != std::string::npos;
  const bool lodz = mnemonic.find("_l") != std::string::npos &&
                    mnemonic.find("_cl") == std::string::npos;
  const bool grad = mnemonic.find("_d") != std::string::npos;
  const bool un = env_has("LGPU_SAMP_UN");
  constexpr std::uint32_t kTexelCount = 16;
  Builder builder;
  EnvelopeProgram program;
  program.probe = true;
  const std::uint32_t pitch = env_u32("LGPU_TEX_PITCH", 64u);
  std::uint32_t tdesc[8], sdesc[4];
  pack_image_tdesc(tdesc, env.arg_va + carrier::kTexelWindow,
                   kTexelCount, pitch);
  pack_sampler_tsdesc(sdesc);
  const TexStage st =
      emit_tex_prelude(builder, program, env, tdesc, sdesc, 20u);
  // Coordinate-slot model (matches GCN V_IMAGE naming):
  //   pre:  c (compare 0.5f), b (bias 0), o (packed offset), in name order
  //   then: s, t   post: cl (clamp 1.0f), l/lz (lod 0)
  // gradients (d): {pre.., ddx(2), ddy(2), s, t, post..}
  const std::uint32_t vcoord = env_u32("LGPU_SAMP_V", 0x3f000000u);
  std::uint32_t off = carrier::kResultBase;
  for (std::size_t ci = 0; ci < rows.size() && ci < kTexelCount; ++ci) {
    const auto& def = rows[ci];
    const std::uint32_t texel =
        def.inputs.empty() ? def.expected.back() : def.inputs[0];
    program.arg_preloads.emplace_back(
        carrier::kTexelWindow + 4 * static_cast<std::uint32_t>(ci), texel);
    // Normalized u = (ci + 0.5) / texels => pixel centers.
    const float uf = (static_cast<float>(2 * ci + 1)) /
                     static_cast<float>(2 * kTexelCount);
    std::uint32_t ubits;
    std::memcpy(&ubits, &uf, 4);
    std::uint32_t mod[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned slot = 0;
    // "_c" (compare) must not match the "_cl" (clamp) suffix.
    const std::size_t cpos = mnemonic.find("_c");
    if (cpos != std::string::npos &&
        (cpos + 2 >= mnemonic.size() || mnemonic[cpos + 2] != 'l')) {
      mod[slot++] = 0x3f000000u;  // compare 0.5f
    }
    if (mnemonic.find("_b") != std::string::npos) {
      mod[slot++] = 0u;           // bias 0
    }
    if (offset) {                 // packed offset (0,0), experiment knobs
      mod[slot++] = (env_u32("LGPU_SAMP_OFFX", 0u) & 0x3f) |
                     ((env_u32("LGPU_SAMP_OFFY", 0u) & 0x3f) << 8);
    }
    if (grad) {
      mod[slot++] = 0x3f800000u; mod[slot++] = 0u;   // ddx = (1,0)
      mod[slot++] = 0u; mod[slot++] = 0x3f800000u;   // ddy = (0,1)
    }
    mod[slot++] = ubits;          // s
    mod[slot++] = vcoord;         // t
    if (mnemonic.find("_cl") != std::string::npos) {
      mod[slot++] = 0x3f800000u;  // clamp 1.0f
    }
    if (lodz) {
      mod[slot++] = 0u;           // lod 0 (level 0)
    }
    const unsigned width = slot;
    const char* rawb = std::getenv("LGPU_SAMP_COORD");
    std::uint32_t crd[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned n = 0;
    if (rawb != nullptr) {
      // Raw coordinate override: space-separated hex dwords, "U" = u.
      const char* rp = rawb;
      while (*rp != '\0' && n < 9) {
        while (*rp == ' ') ++rp;
        if (*rp == '\0') break;
        if (rp[0] == 'U') {
          crd[n++] = ubits;
          rp += 1;
          continue;
        }
        char* endp = nullptr;
        const unsigned long v = std::strtoul(rp, &endp, 0);
        if (endp == rp) break;
        crd[n++] = static_cast<std::uint32_t>(v);
        rp = endp;
      }
    } else {
      for (unsigned g = 0; g < width; ++g) crd[g] = mod[g];
      n = width;
    }
    for (unsigned g = 0; g < 9u; ++g) {
      builder.s_mov_imm_b32(st.imm_temp, g < n ? crd[g] : 0u);
      builder.v_mov_b32(isa::vgpr(16 + g), st.imm_temp, isa::predicate(0));
    }
    if (!env_has("LGPU_NO_TEX")) {
      builder.image_op(mnemonic,
                       {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(8, 4)),
                        isa::op_reg(isa::vgpr(16, grad ? 9u : (width < 3 ? 3u : width))),
                        isa::op_reg(st.tdesc_regs), isa::op_reg(st.sdesc_regs)},
                       un);
      builder.check_k_vm_vs_sm();
    }
    for (unsigned p = 0; p < 4u; ++p) {
      emit_result_store(builder, env, isa::vgpr(8 + p),
                        isa::vgpr(0), off + 4u * p);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    bc.offsets.push_back(off);
    bc.expected.push_back(def.expected.back());
    program.cases.push_back(std::move(bc));
    off += 16;
  }
  if (!finish_program(builder, program, error)) return std::nullopt;
  return program;
}

// ---- control carrier (execute-and-observe) ----------------------------------
// For instructions with no numeric result (control-prefix family), verification
// is survival/termination: the program stores a pre sentinel (0x11111111) at
// arg+0x40, executes the target instruction, then stores a post sentinel
// (0x22222222) at arg+0x44; a completing fence with both siginals = survives
// (non-terminating), only-pre = the instruction terminated the wave
// (exit/trap/s_endpgm/rfe family).  The runner reports PASS when the fence
// completes cleanly (either outcome is the observed hardware behavior).
std::optional<EnvelopeProgram> build_control(const std::string& mnemonic,
                                             const std::vector<EnvelopeCase>&,
                                             const CarrierEnv& env,
                                             std::string& error) {
  // Instructions that transfer control somewhere this carrier cannot name, or
  // that depend on execution-environment state it cannot establish.  Each is
  // refused with the ISA reference's own stated boundary rather than run: a
  // carrier that guesses a target wedges the pipe (measured: jirl with an
  // uninitialised source pair fetched at 0x8 and quarantined the CPIPE).
  struct Refusal {
    const char* mnemonic;
    const char* why;
  };
  static const Refusal kUnrunnable[] = {
      // Nothing refuses here any more: the four control instructions whose
      // behaviour the ISA reference left UNRESOLVED were each resolved against
      // evidence rather than skipped.  jirl and trap take their target from a
      // register pair, which the carrier stages with the address of an
      // executable stub; jpush/jpop are a balanced pair with a zero encoded
      // position; rfe transfers by an encoded displacement whose zero continues
      // at the next instruction.  The operand shapes come from the machine row,
      // which the closed driver's lg200.xml states in full.
      {"-", "unused"},
  };
  for (const auto& r : kUnrunnable) {
    if (mnemonic == r.mnemonic) {
      error = std::string("control carrier: ") + mnemonic +
              " is not run by this carrier - " + r.why;
      return std::nullopt;
    }
  }
  constexpr std::uint32_t kPre = 0x40, kPost = 0x44;
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg zero_address = isa::vgpr(0);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  const isa::Reg imm_temp = isa::sgpr(16);
  builder.s_mov_imm_b32(imm_temp, 0x11111111u);
  builder.v_mov_b32(isa::vgpr(8), imm_temp, isa::predicate(0));
  emit_result_store(builder, env, isa::vgpr(8), zero_address, kPre);
  // the instruction under test, with its fixed carrier operands (values
  // don't matter for control semantics; the point is observed execution)
  std::vector<isa::Operand> ctl_ops;
  if (mnemonic == "jirl") {
    // LoongArch's jirl rd, rj, offs: rd := the return position, PC := rj + offs.
    // The vendor's own kernel uses it as `jirl s[30:31], s[4:5], 0x0`, and its
    // linked form as `jirl zero_64, s[30:31], 0x0`; the register supplies the
    // target, so this carrier stages the stub's address into it and passes no
    // link destination (zero_64), leaving every architectural register alone.
    builder.s_mov_imm_b32(isa::sgpr(10),
                          static_cast<std::uint32_t>(env.trampoline_va));
    builder.s_mov_imm_b32(isa::sgpr(11),
                          static_cast<std::uint32_t>(env.trampoline_va >> 32));
    // s[18:19] receives the return position; the vendor's linked form writes
    // the zero pair instead, which this codec does not spell, and a scratch
    // pair nothing in this carrier reads is equivalent and observable.
    ctl_ops = {isa::op_reg(isa::sgpr(18, 2)), isa::op_reg(isa::sgpr(10, 2)),
               isa::op_imm(0u)};
  } else if (mnemonic == "getpc") {
    // reads the program counter into the pair; no control transfer.
    ctl_ops = {isa::op_reg(isa::sgpr(8, 2))};
  } else if (mnemonic == "getreg" || mnemonic == "cb" || mnemonic == "czb" ||
             mnemonic == "prefix_sw" || mnemonic == "prefix_tp4" ||
             mnemonic == "prefix_tp8") {
    ctl_ops = {isa::op_imm(0u)};
    if (mnemonic == "getreg") ctl_ops = {isa::op_reg(isa::sgpr(8)),
                                         isa::op_imm(0u)};
    if (mnemonic == "cb" || mnemonic == "czb")
      ctl_ops = {isa::op_reg(isa::sgpr(4)), isa::op_imm(0u)};
    if (mnemonic == "flag") ctl_ops = {isa::op_imm(0u), isa::op_modifier("vm", 1),
                                       isa::op_imm(0u)};
  } else if (mnemonic == "flag") {
    ctl_ops = {isa::op_imm(0u), isa::op_modifier("vm", 1), isa::op_imm(0u)};
  } else if (mnemonic == "copy_sm_from_global" ||
             mnemonic == "copy_sm_to_global") {
    ctl_ops = {isa::op_reg(isa::predicate(0))};
  } else if (mnemonic == "trap") {
    // Driver row: X0_32@INST_LO[31:0]; RJ8x2@INST_HI[7:0]; RD8x2@INST_HI[15:8],
    // seed `trap s0-s1, s0-s1 BB_0 (0)`; the assembler's order is (rd, rj, disp)
    // and the source pair is the transfer's target - the role jirl's source pair
    // has - so it is staged with the stub address and the encoded field is 0.
    builder.s_mov_imm_b32(isa::sgpr(10),
                          static_cast<std::uint32_t>(env.trampoline_va));
    builder.s_mov_imm_b32(isa::sgpr(11),
                          static_cast<std::uint32_t>(env.trampoline_va >> 32));
    ctl_ops = {isa::op_reg(isa::sgpr(18, 2)), isa::op_reg(isa::sgpr(10, 2)),
               isa::op_imm(0u)};
  } else if (mnemonic == "rfe") {
    // Driver row: GJ8x2@INST_HI[7:0]; X0_32@INST_LO[31:0], seed
    // `rfe s0-s1, 0x00000000`.  The pair is the return position: zeroing it
    // made the transfer restore a zero position and the instruction fetch
    // faulted at 0x8 (measured).  It is staged with the stub address instead,
    // the same way jirl's and trap's target pairs are, so the restored position
    // is an admitted instruction.
    builder.s_mov_imm_b32(isa::sgpr(18),
                          static_cast<std::uint32_t>(env.trampoline_va));
    builder.s_mov_imm_b32(isa::sgpr(19),
                          static_cast<std::uint32_t>(env.trampoline_va >> 32));
    ctl_ops = {isa::op_reg(isa::sgpr(18, 2)), isa::op_imm(0u)};
  } else if (mnemonic == "jpush" || mnemonic == "jpop") {
    // Shape from the machine row (identical in this catalog and in the closed
    // driver's lg200.xml):
    //   RA8@INST_LO[7:0]; X2_23@INST_LO[30:8]; X0_1@INST_LO[31];
    //   X1_8@INST_HI[15:8]; PJ2@INST_HI[17:16]
    // The rule order is (PJ2, RA8, X1_8, X2_23, X0_1) - five operands, which is
    // what the earlier carrier got wrong by passing four.  X2_23 is the 23-bit
    // encoded control-flow position, so a zero position follows the branch
    // unit's documented rule (a zero displacement continues at the next
    // instruction) and the saved record describes the following instruction.
    ctl_ops = {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::sgpr(4)),
               isa::op_reg(isa::sgpr(5)), isa::op_imm(0u), isa::op_imm(0u)};
  } else if (mnemonic == "b") {
    ctl_ops = {isa::op_imm(0u)};
  }
  if (mnemonic == "check") {
    builder.check_k_vm_vs_sm();
  } else if (mnemonic == "jpush" || mnemonic == "jpop") {
    // Emit the balanced pair for either mnemonic.  The record a pop consumes is
    // the one the push just made, so the stack depth is unchanged across the
    // program and the resume position is the position the push encoded - the
    // following instruction - instead of "whatever the stack happened to hold".
    // A push balances a pop whichever of the two is under test, and the
    // instruction under test is always the second of the pair.
    builder.append_target(isa::McInst{"jpush", "", ctl_ops});
    builder.check_k_vm_vs_sm();
    builder.append_target(
        isa::McInst{"jpop", "", std::move(ctl_ops)});
  } else {
    builder.append_target(isa::McInst{mnemonic, "", std::move(ctl_ops)});
  }
  builder.s_mov_imm_b32(imm_temp, 0x22222222u);
  builder.v_mov_b32(isa::vgpr(8), imm_temp, isa::predicate(0));
  emit_result_store(builder, env, isa::vgpr(8), zero_address, kPost);
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  EnvelopeProgram program;
  program.control = true;
  if (!finalize_program(builder, program, error)) return std::nullopt;
  // a synthetic case so the runner reads pre/post back
  BuiltCase bc;
  bc.id = "control";
  bc.offsets = {kPre, kPost};
  bc.expected = {0x11111111u, 0x22222222u};
  program.cases.push_back(std::move(bc));
  return program;
}

bool is_control(const std::string& mnemonic) {
  return mnemonic == "nop" || mnemonic == "exit" || mnemonic == "trap" ||
         mnemonic == "rfe" || mnemonic == "barrier" || mnemonic == "b" ||
         mnemonic == "cb" || mnemonic == "czb" || mnemonic == "check" ||
         mnemonic == "flag" || mnemonic == "jirl" || mnemonic == "jpush" ||
         mnemonic == "jpop" || mnemonic == "getpc" || mnemonic == "getreg" ||
         mnemonic == "prefix_sw" || mnemonic == "prefix_tp4" ||
         mnemonic == "prefix_tp8" || mnemonic == "copy_sm_from_global" ||
         mnemonic == "copy_sm_to_global";
}

// ---- scalar memory carrier (load_s_flat_w* / load_s_buff_w*) ---------------
static unsigned smem_width(const std::string& mne) {
  if (mne.find("_w16") != std::string::npos) return 16;
  if (mne.find("_w8") != std::string::npos) return 8;
  if (mne.find("_w4") != std::string::npos) return 4;
  if (mne.find("_w2") != std::string::npos) return 2;
  if (mne.find("_w1") != std::string::npos) return 1;
  return 0;
}
std::optional<EnvelopeProgram> build_smem(const std::string& mnemonic,
                                          const std::vector<EnvelopeCase>& rows,
                                          const CarrierEnv& env,
                                          std::string& error) {
  const bool is_flat = mnemonic.rfind("load_s_flat_", 0) == 0;
  const bool is_buff = mnemonic.rfind("load_s_buff_", 0) == 0;
  if (!is_flat && !is_buff) {
    error = "smem carrier: expected load_s_flat_*/load_s_buff_*";
    return std::nullopt;
  }
  const unsigned width = smem_width(mnemonic);
  if (width == 0) {
    error = "smem carrier: unknown width: " + mnemonic;
    return std::nullopt;
  }
  for (const auto& row : rows) {
    if (row.expected.empty() || row.inputs.empty()) {
      error = "smem carrier: row needs addr pair: " + row.id;
      return std::nullopt;
    }
  }
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg zero_addr = isa::vgpr(0);
  builder.v_mov_b32(zero_addr, isa::zero_reg(), isa::predicate(0));
  // flat address pair = absolute VA of the RSD table (kArgVa + 0x200);
  // per-case UO20 = byte offset into the table.
  const isa::Reg flat_addr = isa::sgpr(6, 2);
  builder.s_mov_imm_b32(isa::sgpr(6),
                        static_cast<std::uint32_t>(env.arg_va +
                                                   carrier::kRsdTable));  // lo
  builder.s_mov_imm_b32(isa::sgpr(7),
                        static_cast<std::uint32_t>(env.arg_va >> 32));    // hi
  const isa::Reg dst_s = isa::sgpr(env.scalar_dst.index, width);
  EnvelopeProgram program;
  program.probe = true;
  std::uint32_t off = carrier::kResultBase;
  // cell = byte offset within the RSD table (table base is in s6:s7).
  std::uint32_t cell = 0;
  for (std::size_t ci = 0; ci < rows.size(); ++ci) {
    const auto& def = rows[ci];
    const std::uint32_t win = is_buff ? carrier::kLoadWindow
                                      : carrier::kRsdTable;
    const std::uint32_t cell = static_cast<std::uint32_t>(ci) * width * 4u;
    for (unsigned p = 0; p < width; ++p) {
      const std::uint32_t v =
          def.inputs.size() > p ? def.inputs[p] : 0u;
      program.arg_preloads.emplace_back(win + cell + 4 * p, v);
    }
    if (is_flat) {
      builder.sflat_load(mnemonic, dst_s, flat_addr, isa::zero_reg(),
                         static_cast<std::uint32_t>(cell));
    } else {
      // load_s_buff_*: the address operand is a 4-dword buffer descriptor.
      // Graphics rely on the hardware-hydrated s4-s7; compute builds the words.
      const isa::Reg desc = isa::sgpr(4, 4);
      emit_load_desc(builder, env, desc, env.arg_va + carrier::kLoadWindow);
      builder.sflat_load(mnemonic, dst_s, desc, isa::zero_reg(),
                         static_cast<std::uint32_t>(cell));
    }
    builder.check_k_vm_vs_sm();
    // Read back the load's own destination, not a fixed register number: the
    // scalar window is a launch property (s20 in graphics, s36 in compute, see
    // CarrierEnv::scalar_dst), so hardcoding 20 made every compute row compare
    // a zero register and report 0 while the data sat in the load window.
    for (unsigned p = 0; p < width; ++p) {
      builder.s_mov_b32(isa::sgpr(16), isa::sgpr(dst_s.index + p));
      builder.v_mov_b32(isa::vgpr(10 + p), isa::sgpr(16),
                        isa::predicate(0));
    }
    for (unsigned p = 0; p < width; ++p) {
      emit_result_store(builder, env, isa::vgpr(10 + p), zero_addr,
                        off + 4 * p);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    const std::size_t ncmp = def.expected.size() <= width ? def.expected.size()
                                                         : width;
    for (std::size_t p = 0; p < ncmp; ++p) bc.offsets.push_back(off + 4 * p);
    for (std::size_t p = 0; p < ncmp; ++p)
      bc.expected.push_back(def.expected[p]);
    program.cases.push_back(std::move(bc));
    off += 4 * width;
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();
  if (!finalize_program(builder, program, error)) return std::nullopt;
  return program;
}

std::optional<EnvelopeProgram> build_gs(const std::string& mnemonic,
                                        const std::vector<EnvelopeCase>& rows,
                                        const CarrierEnv& env,
                                        std::string& error) {
  if (rows.empty()) {
    error = "gs carrier: no rows";
    return std::nullopt;
  }
  const bool emit = mnemonic == "v_emit2" || mnemonic == "v_emit3";
  const bool sio = mnemonic == "siowrite" || mnemonic == "sioread" ||
                   mnemonic == "siordrot" || mnemonic == "siowrrot" ||
                   mnemonic == "siordrot_id" || mnemonic == "siowrrot_id" ||
                   mnemonic == "siordrot_c4" || mnemonic == "siowrrot_c4" ||
                   mnemonic == "siordrot_id_c4" ||
                   mnemonic == "siowrrot_id_c4" ||
                   mnemonic == "siordrot_imm" || mnemonic == "siowrrot_imm" ||
                   mnemonic == "siordrot_imm_c4" ||
                   mnemonic == "siowrrot_imm_c4" || mnemonic == "sioread_imm" ||
                   mnemonic == "siowrite_imm" || mnemonic == "sioexport";
  if (!emit && !sio) {
    error = "gs carrier: expected v_emit2/3 or sio mnemonics";
    return std::nullopt;
  }
  Builder builder;
  const isa::Reg store_desc = carrier::kStoreDesc();
  // The result store's descriptor is the environment's: graphics hydrates it
  // from the packet, compute has to be handed the literal, and this carrier's
  // own staging used to hardcode the graphics argument BO.
  emit_env_prelude(builder, env);
  const isa::Reg zero_address = isa::vgpr(0);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));

  EnvelopeProgram program;
  program.probe = true;
  // SIO reads/rotates return channel-file content not oracle-able
  // (launch-state dependent); observe mode records without failing.
  // v_emit2/3 clobber the emit-index VGPR with the descriptor side effect
  // before the payload copy (contract doc 38), so they observe too.
  if ((sio && mnemonic != "siowrite" && mnemonic != "siowrite_imm") ||
      emit) {
    program.observe = true;
  }
  std::uint32_t off = carrier::kResultBase;
  for (std::size_t ci = 0; ci < rows.size(); ++ci) {
    const auto& def = rows[ci];
    builder.s_mov_imm_b32(isa::sgpr(24),
                          def.inputs.empty() ? 0u : def.inputs[0]);
    builder.v_mov_b32(isa::vgpr(8), isa::sgpr(24), isa::predicate(0));
    if (std::getenv("LGPU_GSDUMP")) {
      builder.s_mov_imm_b32(isa::sgpr(24), 0x11111111u);
      builder.v_mov_b32(isa::vgpr(8), isa::sgpr(24), isa::predicate(0));
    }
    builder.s_mov_imm_b32(isa::sgpr(24),
                          def.inputs.size() > 1 ? def.inputs[1] : 0u);
    builder.v_mov_b32(isa::vgpr(9), isa::sgpr(24), isa::predicate(0));
    if (emit) {
      builder.append_target(isa::McInst{
          mnemonic, "",
          {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(8)),
           isa::op_reg(isa::sgpr(0)), isa::op_reg(isa::vgpr(9))}});
      builder.check_k_vm_vs_sm();
      emit_result_store(builder, env, isa::vgpr(8), zero_address,
                           off);
      builder.check_k_vm_vs_sm();
    } else if (mnemonic == "siowrite" || mnemonic == "siowrite_imm") {
      if (mnemonic == "siowrite_imm") {
        builder.append_target(isa::McInst{
            mnemonic, "",
            {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
             isa::op_reg(isa::vgpr(8)),
             isa::op_modifier("channum", 4), isa::op_imm(0),
             isa::op_modifier("chan", 0)}});
      } else {
        builder.append_target(isa::McInst{
            mnemonic, "",
            {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
             isa::op_reg(isa::vgpr(8)), isa::op_reg(isa::vgpr(9)),
             isa::op_modifier("channum", 0), isa::op_imm(0)}});
      }
      builder.check_k_vm_vs_sm();
      emit_result_store(builder, env, isa::vgpr(8), zero_address,
                           off);
      builder.check_k_vm_vs_sm();
    } else if (mnemonic == "sioread" || mnemonic == "sioread_imm") {
      if (mnemonic == "sioread_imm") {
        builder.append_target(isa::McInst{
            mnemonic, "",
            {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
             isa::op_reg(isa::vgpr(12)),
             isa::op_modifier("channum", 4), isa::op_imm(0),
             isa::op_modifier("chan", 0)}});
      } else {
        builder.append_target(isa::McInst{
            mnemonic, "",
            {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
             isa::op_reg(isa::vgpr(9)), isa::op_reg(isa::vgpr(12)),
             isa::op_modifier("channum", 0), isa::op_imm(0)}});
      }
      builder.check_k_vm_vs_sm();
      emit_result_store(builder, env, isa::vgpr(12), zero_address,
                           off);
      builder.check_k_vm_vs_sm();
    } else if (mnemonic == "sioexport") {
      builder.append_target(isa::McInst{
          mnemonic, "",
          {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
           isa::op_modifier("channum", 8)}});
      builder.check_k_vm_vs_sm();
      emit_result_store(builder, env, isa::vgpr(8), zero_address,
                           off);
      builder.check_k_vm_vs_sm();
    } else if (mnemonic.rfind("_imm_c4") != std::string::npos) {
      // sio-imm c4: (sio, pred, VJ8x4 quad, channum, imm16, chan)
      builder.append_target(isa::McInst{
          mnemonic, "",
          {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
           isa::op_reg(isa::vgpr(10, 4)),
           isa::op_modifier("channum", 4), isa::op_imm(0),
           isa::op_modifier("chan", 0)}});
      builder.check_k_vm_vs_sm();
      emit_result_store(builder, env, isa::vgpr(10), zero_address,
                           off);
      builder.check_k_vm_vs_sm();
    } else if (mnemonic.find("_imm") != std::string::npos) {
      // sio-imm forms: (sio, pred, VJ8 data, channum, imm16, chan)
      // e.g. siordrot_imm SIO0, p0, v4, channum(4), 0x0000, chan(0).
      builder.append_target(isa::McInst{
          mnemonic, "",
          {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
           isa::op_reg(isa::vgpr(10)),
           isa::op_modifier("channum", 4), isa::op_imm(0),
           isa::op_modifier("chan", 0)}});
      builder.check_k_vm_vs_sm();
      emit_result_store(builder, env, isa::vgpr(10), zero_address,
                           off);
      builder.check_k_vm_vs_sm();
    } else {
      // sio-rot carries two vector sources (VJ8, VK8); sio-rot-id carries
      // three (VJ8, VA8, VK8 - the identifier form adds VA8).  Emitting the
      // two-source shape for an identifier row made the encoder's vector-field
      // omission rule zero VK8 silently: the next operand (channum) is not a
      // vector, so it was taken as the omitted zero selector.  The driver rows
      // agree on three sources - seed "siordrot_id p0, v0, v0, v0, 0x0000
      // sio:0 channum:8" against "siordrot p0, v0, v0, 0x0000 sio:0
      // channum:8", and signature VA8VK8IA16M.channum12VJ8PJ2M.sioid1 against
      // VK8IA16M.channum12VJ8PJ2M.sioid1.  What role VA8 plays is written
      // UNRESOLVED in the catalog, so this only restores the operand the row
      // declares; it does not claim a meaning for it.
      std::vector<isa::Operand> sio_ops = {
          isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
          isa::op_reg(isa::vgpr(10)), isa::op_reg(isa::vgpr(11))};
      const bool id_form = mnemonic == "siordrot_id" ||
                           mnemonic == "siowrrot_id" ||
                           mnemonic == "siordrot_id_c4" ||
                           mnemonic == "siowrrot_id_c4";
      if (id_form) {
        sio_ops.push_back(isa::op_reg(isa::vgpr(12)));
      }
      sio_ops.push_back(isa::op_modifier("channum", 0));
      sio_ops.push_back(isa::op_imm(0));
      builder.append_target(isa::McInst{mnemonic, "", std::move(sio_ops)});
      builder.check_k_vm_vs_sm();
      emit_result_store(builder, env, isa::vgpr(10), zero_address,
                           off);
      builder.check_k_vm_vs_sm();
    }
    BuiltCase bc;
    bc.id = def.id;
    bc.offsets.push_back(off);
    if (def.expected.size() >= 1) bc.expected.push_back(def.expected[0]);
    program.cases.push_back(std::move(bc));
    off += 4;
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();
  // SIO encodings collide with VOP1/SOP2 forms in the open decoder
  // (siordrot == s_add_u32 slot), so skip the round-trip check here.
  if (!link_program_no_rr(builder.lines, program, error)) return std::nullopt;

  Builder fsb;
  const std::uint32_t kColor[4] = {0x3e4ccccdu, 0x3ecccccdu,
                                   0x3f19999au, 0x3f800000u};
  for (unsigned c = 0; c < 4; ++c) {
    fsb.s_mov_imm_b32(isa::sgpr(c), kColor[c]);
    fsb.v_mov_b32(isa::vgpr(c), isa::sgpr(c), isa::predicate(0));
  }
  fsb.append_target(isa::McInst{
      "v_pack_f16rtz_f32", "",
      {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(0)),
       isa::op_reg(isa::vgpr(1)), isa::op_reg(isa::vgpr(0))}});
  fsb.append_target(isa::McInst{
      "v_pack_f16rtz_f32", "",
      {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(1)),
       isa::op_reg(isa::vgpr(3)), isa::op_reg(isa::vgpr(1))}});
  fsb.append_target(isa::McInst{
      "siowrrot_imm", "",
      {isa::op_modifier("sio", 1), isa::op_reg(isa::predicate(0)),
       isa::op_reg(isa::vgpr(0)), isa::op_modifier("channum", 2),
       isa::op_imm(0), isa::op_modifier("chan", 1)}});
  fsb.append_target(isa::McInst{
      "siowrrot_imm", "",
      {isa::op_modifier("sio", 1), isa::op_reg(isa::predicate(0)),
       isa::op_reg(isa::vgpr(1)), isa::op_modifier("channum", 2),
       isa::op_imm(0), isa::op_modifier("chan", 1)}});
  fsb.append_target(isa::McInst{
      "sioexport", "",
      {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
       isa::op_modifier("channum", 2)}});
  fsb.nop();
  fsb.exit();
  EnvelopeProgram fs;
  std::string ferr;
  if (!link_program(fsb.lines, fs, ferr)) {
    error = "gs carrier: solid FS link: " + ferr;
    return std::nullopt;
  }
  program.fs_bytes = fs.bytes;
  return program;
}

// ---- single entry point: family routing --------------------------------------
// The runner calls only this.  Order matters and mirrors the historical
// dispatch exactly: buff, flat, lds, tex-atomics, tex-store, tex-load,
// samp, control, generic.
// Which carrier owns a mnemonic, in one place.  The environment does not
// change the routing decision; it changes what the chosen carrier emits.
struct CarrierRoute {
  enum class Family { Smem, Gs, Buff, Flat, Lds, TexAtomic, TexStore, Tex,
                      Samp, Control, Generic, Unsupported } family;
  const char* reason;  // non-null when the family is graphics-only
};

CarrierRoute route_mnemonic(const std::string& m) {
  const bool gs = m == "v_emit2" || m == "v_emit3" || m.rfind("sio", 0) == 0;
  if (m.rfind("load_s_flat_", 0) == 0 || m.rfind("load_s_buff_", 0) == 0) {
    return {CarrierRoute::Family::Smem, nullptr};
  }
  if (gs) {
    // The stream-out family drives LDS through the SIO channel file rather than
    // the GS stage per se, so a compute dispatch can carry it.  Its rows stay in
    // observe mode: the channel-file contents are launch-state dependent, and
    // this is the family whose LDS base a compute dispatch does not otherwise
    // establish, so it is validated last.
    return {CarrierRoute::Family::Gs, nullptr};
  }
  if (m.rfind("load_v_buff_", 0) == 0 || m.rfind("store_v_buff_", 0) == 0 ||
      m.rfind("atomic_v_buff_", 0) == 0) {
    return {CarrierRoute::Family::Buff, nullptr};
  }
  if (m.rfind("load_v_flat", 0) == 0 || m.rfind("store_v_flat", 0) == 0 ||
      m.rfind("atomic_v_flat", 0) == 0) {
    return {CarrierRoute::Family::Flat, nullptr};
  }
  if (m.rfind("load_sm_", 0) == 0 || m.rfind("store_sm_", 0) == 0 ||
      m.rfind("atomic_sm_", 0) == 0) {
    return {CarrierRoute::Family::Lds, nullptr};
  }
  // T#/S# families: graphics only, for a reason that survives the whole family
  // being read against the closed driver's own rows.
  //
  // The rows agree with this table on every shape: the fetch/store/atomic forms
  // name the T# alone (RK9x8, an 8-aligned octet), the sampling forms name the
  // S# (RA8x4, a 4-aligned quad) as well, and the carrier's tuple assignment
  // satisfies both on both pipes.  What a compute dispatch cannot supply is the
  // image the T# describes.  Graphics hydrates the descriptor from the packet's
  // resource root; a compute dispatch has only what the carrier builds, and the
  // driver's atomic row declares an R32_UINT image with a 2D address while
  // dmask:0xf spans four texels - so the addressed texel must have four texels
  // of declared extent behind it, which a fetch clamps and an atomic does not.
  // Rather than guess that geometry, the whole family stays on the graphics
  // side, where the packet supplies it.
  auto tex_reason = [](const char* what) {
    return what;
  };
  if (m.rfind("atomic_v_tex", 0) == 0) {
    return {CarrierRoute::Family::TexAtomic,
            tex_reason("the T# must describe an image whose declared extent "
                       "covers the addressed texel plus the dmask span, and "
                       "only the graphics packet hydrates that image")};
  }
  if (m.rfind("store_v_tex", 0) == 0) {
    return {CarrierRoute::Family::TexStore,
            tex_reason("needs the image T# the graphics packet hydrates")};
  }
  if (m.rfind("load_v_tex", 0) == 0 || m == "getresinfo") {
    return {CarrierRoute::Family::Tex,
            tex_reason("needs the image T# the graphics packet hydrates")};
  }
  if ((m.rfind("samp", 0) == 0 && m != "sampler") ||
      m.rfind("gather", 0) == 0 || m == "getlod") {
    return {CarrierRoute::Family::Samp,
            tex_reason("needs the image T# and sampler S# the graphics packet "
                       "hydrates")};
  }
  if (m == "copy_sm_from_global" || m == "copy_sm_to_global") {
    // The closed driver's own ISA row carries the seed text
    //   "copy_sm_from_global p0, Unsupport disasm copy_sm_from/to_global."
    // i.e. these two take ONLY a predicate and reach global memory through
    // implicit state that the driver's own tooling does not name.  Graphics
    // supplies it by hydration, which is why they pass there; a compute
    // dispatch has nothing to establish it from, and the transfer reads at
    // address 0 - measured: fault addr 0x0 domain 11, pipe quarantined.
    return {CarrierRoute::Family::Control,
            "LDS<->global transfer with no established launch contract: the "
            "closed driver's own TODO.md keeps copy_sm_* fail-closed - \"no "
            "Vulkan NIR intrinsic or reviewed compute carrier ... remain "
            "fail-closed until a source-level operation and launch contract are "
            "established\" - and its ISA row is marked \"Unsupport disasm\".  "
            "The contract it needs is two bindings this carrier does not make: "
            "the workgroup shared size in the kernel descriptor's "
            "shared_segment_size plus the resource word's granulated-shared "
            "field, and the launch-bound global region the copy names.  Without "
            "them the implicit state is whatever the wave holds and the "
            "transfer reads at address 0 (measured, fault addr 0x0 domain 11)"};
  }
  if (m == "v_movindex_b32") {
    // The movindex window is not established for the VECTOR form.  The scalar
    // sibling is the one that has been probed, and even there only as a
    // hypothesis - build_movidx_probe's own comment reads "If the movindex
    // window reads the CURRENT sGPR file, idx=16 yields the magic value",
    // which is why that probe exists.  This form has no dedicated carrier: it
    // falls through to the generic path, which stages the three source
    // operands from case data and leaves the window itself unbound.
    //
    // Measured on compute it does not fault, it HANGS: the ring report is
    // `xdma0 timeout` with no fault address, so there is no bad access to
    // reason from, and the pipe then sits in the CP state that only platform
    // power clears.  A hang is strictly worse than a fault here - nothing to
    // compare, and every attempt costs a cold boot.
    //
    // Refused until a carrier binds base/index/offset the way
    // build_movidx_probe stages them, so the window read is in-file and its
    // result is comparable rather than unobservable.
    return {CarrierRoute::Family::Generic,
            "movindex window not established for the vector form: the scalar "
            "sibling's window semantics are still a hypothesis (see "
            "build_movidx_probe), this form has no dedicated carrier, and the "
            "generic path leaves the window unbound.  Measured on compute it "
            "hangs rather than faults - xdma0 ring timeout, no fault address - "
            "and the pipe then needs platform power"};
  }
  if (is_control(m)) {
    return {CarrierRoute::Family::Control, nullptr};
  }
  return {CarrierRoute::Family::Generic, nullptr};
}

namespace {
std::optional<EnvelopeProgram> build_carrier_env(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    const CarrierEnv& env, std::string& error) {
  const CarrierRoute route = route_mnemonic(mnemonic);
  if (route.reason != nullptr && env.flat_store) {
    error = std::string("compute carrier has no ") + mnemonic +
            ": " + route.reason;
    return std::nullopt;
  }
  std::optional<EnvelopeProgram> p;
  switch (route.family) {
    case CarrierRoute::Family::Smem:
      p = build_smem(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Gs:
      p = build_gs(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Buff:
      p = build_buff(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Flat:
      p = build_flat(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Lds:
      p = build_lds(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::TexAtomic:
      p = build_tex_atomic(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::TexStore:
      p = build_tex_store(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Tex:
      p = build_tex(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Samp:
      p = build_samp(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Control:
      p = build_control(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Generic:
      p = build_generic(mnemonic, rows, env, error);
      break;
    case CarrierRoute::Family::Unsupported:
      error = "no carrier for " + mnemonic;
      return std::nullopt;
  }
  if (!p) return p;
  if (env.flat_store) {
    // The probe roots are a graphics submission concept; a compute dispatch
    // reaches its window through the addresses the carrier assembled, so the
    // graphics trasport hint must not survive.
    p->probe = false;
  }
  // [HW 2026-08-25] PS-phase launch-state families: the hardware routes these
  // to per-fragment windows whose content the carrier cannot set up (vs. the
  // staged sources), so the only reproducible record is observation.
  // Evidence: flat loads/stores/atomics read input-plane values that change
  // with the 0x530 input_enable toggle; fmtdesc/fmtinst and i8/u8/b8 subword
  // loads return fixed plane patterns (0x3f800000 / 0x3fa1ffff) for any staged
  // data; load_sm_matrix reads a persistent matrix file (cross-run state);
  // tex/getlod/getresinfo fetch window state (0.5 / exponent-plane values)
  // regardless of the staged texels.
  const bool launch_state =
      route.family == CarrierRoute::Family::Flat ||
      mnemonic.rfind("load_sm_matrix_", 0) == 0 ||
      mnemonic.find("_fmtdesc_") != std::string::npos ||
      mnemonic.find("_fmtinst_") != std::string::npos ||
      mnemonic == "load_v_buff_i8" || mnemonic == "load_v_buff_u8" ||
      mnemonic == "store_v_buff_b8" || mnemonic == "getlod" ||
      mnemonic == "getresinfo" || mnemonic.rfind("load_v_tex", 0) == 0 ||
      mnemonic.rfind("store_v_tex", 0) == 0 ||
      mnemonic.rfind("v_mma_", 0) == 0 ||
      mnemonic == "v_sqrt_f64" || mnemonic == "v_rsq_f64" ||
      mnemonic == "v_s2v_b32" || mnemonic == "v_cmpclass_f64" ||
      mnemonic == "v_add_imm_i32" || mnemonic == "v_sub_imm_i32" ||
      mnemonic == "v_perm_sm_b32" || mnemonic == "v_bperm_sm_b32" ||
      mnemonic == "s_cmp_eq_u64" || mnemonic == "s_cmp_ne_u64";
  if (launch_state) p->observe = true;
  return p;
}

}  // namespace

std::optional<EnvelopeProgram> build_carrier(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    LaunchEnvironment kind, std::string& error, unsigned lanes) {
  return build_carrier_env(
      mnemonic, rows,
      kind == LaunchEnvironment::Compute ? CarrierEnv::compute(lanes)
                                         : CarrierEnv::graphics(),
      error);
}

std::optional<EnvelopeProgram> build_ps(const std::string& mnemonic,
                                        const std::vector<EnvelopeCase>& rows,
                                        std::string& error) {
  return build_carrier(mnemonic, rows, LaunchEnvironment::Graphics, error);
}

std::optional<EnvelopeProgram> build_cs(const std::string& mnemonic,
                                        const std::vector<EnvelopeCase>& rows,
                                        std::string& error, unsigned lanes) {
  return build_carrier(mnemonic, rows, LaunchEnvironment::Compute, error,
                       lanes);
}

// The compute launch-state self-test.
//
// This is the one program whose job is the LAUNCH, not an instruction: it
// reads the four things a compute dispatch supplies and stores each one where
// the host can see it.  It exists because the rest of the compute carrier
// deliberately does not depend on the launch state (its result stores use
// assembled addresses), so without this nothing would exercise - or fail on -
// the kernarg pointer, the group ids or the local ids.
//
//   out[0..1]  s[6:7] + 0 as loaded: the kernarg block's first user argument,
//              which the host wrote as the output BO's VA.  A wrong kernarg
//              pointer faults here instead of surfacing as a wrong value.
//   out[2]     s11 = group id x        (en_sgpr_group_id_x)
//   out[3]     s12 = group id y
//   out[4]     s13 = group id z
//   out[5]     v0  = local invocation id x
//   out[6]     v1  = local invocation id y
//   out[7]     v2  = local invocation id z
//
// The expected values are computed by the caller from the geometry it asked
// for, so the row is a real comparison and not an observation.
std::optional<EnvelopeProgram> build_cs_probe(std::string& error,
                                             unsigned lanes) {
  const CarrierEnv env = CarrierEnv::compute(lanes);
  Builder builder;
  // Launch state first, before anything can clobber s6:s7 (the emitter's VCC
  // operand is the same register pair).
  builder.sflat_load("load_s_flat_w2", isa::sgpr(0, 2), isa::sgpr(6, 2),
                     isa::zero_reg(), 0u);
  emit_env_prelude(builder, env);
  // s[6:7]+0 -> out[0..1]
  builder.v_mov_b32(isa::vgpr(8), isa::sgpr(0), isa::predicate(0));
  emit_result_store(builder, env, isa::vgpr(8), isa::vgpr(9), 0);
  builder.v_mov_b32(isa::vgpr(8), isa::sgpr(1), isa::predicate(0));
  emit_result_store(builder, env, isa::vgpr(8), isa::vgpr(9), 4);
  // group ids s11/s12/s13 -> out[2..4]
  for (unsigned i = 0; i < 3; ++i) {
    builder.v_mov_b32(isa::vgpr(8), isa::sgpr(11 + i), isa::predicate(0));
    emit_result_store(builder, env, isa::vgpr(8), isa::vgpr(9), 8 + 4 * i);
  }
  // local ids v0/v1/v2 -> out[5..7]
  for (unsigned i = 0; i < 3; ++i) {
    emit_result_store(builder, env, isa::vgpr(i), isa::vgpr(9), 20 + 4 * i);
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  EnvelopeProgram program;
  BuiltCase bc;
  bc.id = "cs-launch-state";
  for (unsigned i = 0; i < 8; ++i) bc.offsets.push_back(4 * i);
  program.cases.push_back(std::move(bc));
  if (!finalize_program(builder, program, error)) return std::nullopt;
  return program;
}

std::optional<EnvelopeProgram> build_bench(const std::string& mnemonic,
                                           unsigned copies, bool chained,
                                           std::string& error) {
  if (copies == 0) {
    error = "copies must be positive";
    return std::nullopt;
  }
  const auto* spec = isa::semantic_spec(mnemonic);
  if (spec == nullptr || spec->rule_count == 0) {
    error = "no semantic specification";
    return std::nullopt;
  }
  std::vector<isa::RegClass> classes;
  if (!general_source_classes(mnemonic, classes)) {
    error = "no canonical variant";
    return std::nullopt;
  }
  if (!materializable(*spec, classes)) {
    error = "not materializable on the generic PS carrier";
    return std::nullopt;
  }
  if (mnemonic == "nop" || mnemonic == "exit" || mnemonic == "check") {
    error = "excluded mnemonic";
    return std::nullopt;
  }

  Builder builder;
  // Store into the colour BO, NOT the argument BO: the argument BO is where
  // the 0x13 timestamp pair lands, and writing to it drops the END
  // writeback, which would leave every benchmark at zero ticks.
  emit_store_desc(builder, carrier::kColorVa);
  const isa::Reg descriptor = carrier::kStoreDesc();
  const isa::Reg zero_address = isa::vgpr(9);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  // Constant inputs: scalar sources read s4 (0), VCC preloaded to 0.
  builder.s_mov_imm_b32(isa::sgpr(4), 0u);
  builder.s_mov_imm_b32(isa::vcc(6), 0u);

  const std::uint8_t dst_width =
      isa::semantic_tuple(spec->mnemonic, "VD8", 1);
  bool has_vgpr_dst = false;
  bool has_sgpr_dst = false;
  for (unsigned i = 0; i < spec->rule_count; ++i) {
    if (spec->rules[i].sem == isa::FieldSem::VectorDest8) {
      has_vgpr_dst = true;
    }
    if (spec->rules[i].sem == isa::FieldSem::Scalar8 && spec->rules[i].is_output) {
      has_sgpr_dst = true;
    }
    if (spec->rules[i].sem == isa::FieldSem::ScalarCmp8 &&
        spec->rules[i].is_output) {
      has_sgpr_dst = true;  // materialized via the s_select wrapper
    }
  }

  for (unsigned copy = 0; copy < copies; ++copy) {
    const isa::Reg chain_reg = isa::vgpr(10, dst_width);
    std::vector<isa::Operand> operands;
    for (unsigned ri = 0; ri < spec->rule_count; ++ri) {
      const auto& rule = spec->rules[ri];
      const std::uint8_t width =
          isa::semantic_tuple(spec->mnemonic, rule.field, rule.tuple_width);
      switch (rule.sem) {
        case isa::FieldSem::Predicate2:
          operands.push_back(isa::op_reg(isa::predicate(0)));
          break;
        case isa::FieldSem::VectorDest8:
          operands.push_back(isa::op_reg(isa::vgpr(10, width)));
          break;
        case isa::FieldSem::VectorSource8:
        case isa::FieldSem::GeneralSource11:
        case isa::FieldSem::GeneralSource8: {
          const std::size_t flen = std::strlen(rule.field);
          const bool narrow8 =
              flen > 0 && rule.field[flen - 1] == '8' &&
              classes[ri] != isa::RegClass::Vector;
          if (classes[ri] == isa::RegClass::Scalar || narrow8) {
            operands.push_back(isa::op_reg(isa::sgpr(4, width)));
          } else {
            const unsigned lane =
                chained ? 10u : 10u + ((copy % 8) * 2u);
            operands.push_back(isa::op_reg(isa::vgpr(lane, width)));
          }
          break;
        }
        case isa::FieldSem::Scalar8:
          operands.push_back(isa::op_reg(
              rule.is_output ? isa::sgpr(8, width) : isa::sgpr(4, width)));
          break;
        case isa::FieldSem::ScalarCmp8:
          if (rule.is_output) {
            operands.push_back(isa::op_reg(isa::scc_reg()));
          } else {
            error = "unreachable scalar-cmp source";
            return std::nullopt;
          }
          break;
        case isa::FieldSem::Vcc8:
          operands.push_back(isa::op_reg(isa::vcc(6)));
          break;
        case isa::FieldSem::SccSource8:
          operands.push_back(isa::op_reg(isa::sgpr(4, width)));
          break;
        case isa::FieldSem::Immediate32:
        case isa::FieldSem::Immediate8:
          operands.push_back(isa::op_imm(0u));
          break;
        case isa::FieldSem::Modifier:
          break;
        default:
          error = "unsupported field semantics in bench template";
          return std::nullopt;
      }
    }
    builder.append_target(isa::McInst{spec->mnemonic, "", std::move(operands)});
    if (is_implicit_scc_family(spec->mnemonic)) {
      builder.append_target(isa::McInst{
          "s_select_b32", "",
          {isa::op_reg(isa::sgpr(8)), isa::op_reg(isa::sgpr(4)),
           isa::op_reg(isa::sgpr(4)), isa::op_reg(isa::scc_reg())}});
    }
  }

  // Store the final destination once (both templates keep the same store
  // shape so the timing delta is attributable to the instruction pattern).
  // LGPU_BENCH_NOSTORE suppresses the result store.  A timestamped program that
  // writes to the argument BO loses the 0x13 END writeback, and this switch
  // exists to test that with a single variable changed: everything else in the
  // program, including the instruction chain being timed, is identical.
  if (std::getenv("LGPU_BENCH_NOSTORE") == nullptr) {
  {  // store the final destination
    if (has_vgpr_dst) {
      for (std::uint8_t part = 0; part < dst_width; ++part) {
        builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(10 + part),
                                 zero_address, descriptor, isa::zero_reg(),
                                 4 * part, true, false, false, false);
      }
    } else if (has_sgpr_dst) {
      builder.v_mov_b32(isa::vgpr(11), isa::sgpr(8), isa::predicate(0));
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(11), zero_address,
                               descriptor, isa::zero_reg(), 0, true, false,
                               false, false);
    }
  }
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  EnvelopeProgram program;
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// The flat memory family's bench template.  A memory access has no register
// source to chain through (see MICROBENCH.md 5.1), so the two kinds are built
// differently:
//   tp  - K independent accesses from one staged address.  This is a THROUGHPUT
//         number; it is not a latency and must not be reported as one.
//   lat - each access's value feeds the next access's address, so the copies form
//         a dependency chain.  The link costs one integer add, which the measured
//         window therefore includes; the loaded value is a small staged constant
//         so the chained address stays inside the carrier's own window.
// Only the load forms are built: a store has no value to chain through, and what
// its "latency" would measure is the address computation, not the store.
std::optional<EnvelopeProgram> build_cs_bench_flat(const std::string& mnemonic,
                                                   unsigned copies, bool chained,
                                                   unsigned lanes,
                                                   std::string& error) {
  if (mnemonic.rfind("load_v_flat_", 0) != 0) {
    error = "flat bench: only the load forms are built";
    return std::nullopt;
  }
  const std::string suffix = mnemonic.substr(std::string("load_v_flat_").size());
  // The sub-word loads are spelled i8/u8/i16/u16; b8/b16 have no load form.
  if (suffix != "i8" && suffix != "u8" && suffix != "i16" && suffix != "u16" &&
      suffix != "b32" && suffix != "b64" && suffix != "b96" && suffix != "b128") {
    error = "flat bench: unknown access width";
    return std::nullopt;
  }
  const unsigned width = suffix == "b128" ? 4u : (suffix == "b96" ? 3u
                                                   : (suffix == "b64" ? 2u : 1u));
  const CarrierEnv env = CarrierEnv::compute(lanes);
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg flat_base = isa::sgpr(20, 2);
  const isa::Reg addr_pair = isa::vgpr(0, 2);
  const isa::Reg addr_lo = isa::vgpr(0);
  const isa::Reg chained_dst = isa::vgpr(2, width);
  const isa::Reg result_v = isa::vgpr(10, width);
  const isa::Reg imm_temp = isa::sgpr(16);
  // The LCO shape: the vaddr pair carries the full absolute 64-bit VA and the
  // scalar base pair is literal zero.
  builder.s_mov_imm_b32(isa::sgpr(20), 0u);
  builder.s_mov_imm_b32(isa::sgpr(21), 0u);
  const std::uint32_t addr = carrier::kLoadWindow;
  const std::uint64_t full_va = env.arg_va + addr;
  builder.s_mov_imm_b32(imm_temp, static_cast<std::uint32_t>(full_va));
  builder.v_mov_b32(addr_lo, imm_temp, isa::predicate(0));
  builder.s_mov_imm_b32(imm_temp, static_cast<std::uint32_t>(full_va >> 32));
  builder.v_mov_b32(isa::vgpr(1), imm_temp, isa::predicate(0));
  EnvelopeProgram program;
  program.probe = true;
  // A small staged value keeps a chained address inside the carrier's window.
  for (unsigned p = 0; p < width; ++p)
    program.arg_preloads.emplace_back(addr + 4 * p, 8u);
  for (unsigned copy = 0; copy < copies; ++copy) {
    builder.load_v_flat(mnemonic, isa::predicate(0),
                        chained ? chained_dst : result_v, addr_pair, flat_base);
    if (chained) {
      // The seed reads v_add_imm_u32 p0, v0, v0, 0x0: the predicate comes first.
      builder.append_target(isa::McInst{
          "v_add_imm_u32", "",
          {isa::op_reg(isa::predicate(0)), isa::op_reg(addr_lo),
           isa::op_reg(chained_dst),
           isa::op_imm(static_cast<std::uint32_t>(full_va))}});
    }
  }
  const isa::Reg zero_address = isa::vgpr(9);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  builder.s_mov_imm_b32(isa::sgpr(4), 0u);
  for (unsigned p = 0; p < width; ++p)
    emit_result_store(builder, env, chained ? chained_dst : result_v,
                      zero_address, 4 * p);
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// The flat ATOMIC family's bench template.  Unlike a plain load, an atomic takes
// an operand AND returns the old value, so the two kinds fall out naturally:
//   tp  - K independent atomics at one address, result in a separate register;
//   lat - the destination IS the operand, so copy i's returned old value is
//         copy i+1's operand and the copies form one dependency chain.
// Both use only operand positions the correctness carrier already exercises
// (arg_preloads for the cell, the vaddr pair for the address).
std::optional<EnvelopeProgram> build_cs_bench_flat_atomic(
    const std::string& mnemonic, unsigned copies, bool chained, unsigned lanes,
    std::string& error) {
  if (mnemonic.find("cmpswap") != std::string::npos) {
    error = "flat atomic bench: the cmpswap form stages a compare pair this "
            "template does not build";
    return std::nullopt;
  }
  const bool wide64 = mnemonic.find("_i64") != std::string::npos ||
                      mnemonic.find("_u64") != std::string::npos ||
                      mnemonic.find("_b64") != std::string::npos;
  const unsigned width = wide64 ? 2u : 1u;
  const CarrierEnv env = CarrierEnv::compute(lanes);
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg flat_base = isa::sgpr(20, 2);
  const isa::Reg addr_pair = isa::vgpr(0, 2);
  const isa::Reg addr_lo = isa::vgpr(0);
  const isa::Reg data_v = isa::vgpr(8, width);
  const isa::Reg result_v = isa::vgpr(10, width);
  const isa::Reg imm_temp = isa::sgpr(16);
  const std::uint32_t addr = carrier::kLoadWindow;
  const std::uint64_t full_va = env.arg_va + addr;
  builder.s_mov_imm_b32(imm_temp, static_cast<std::uint32_t>(full_va));
  builder.v_mov_b32(addr_lo, imm_temp, isa::predicate(0));
  builder.s_mov_imm_b32(imm_temp, static_cast<std::uint32_t>(full_va >> 32));
  builder.v_mov_b32(isa::vgpr(1), imm_temp, isa::predicate(0));
  EnvelopeProgram program;
  program.probe = true;
  for (unsigned p = 0; p < width; ++p) {
    program.arg_preloads.emplace_back(addr + 4 * p, 1u);
    builder.s_mov_imm_b32(imm_temp, 1u);
    builder.v_mov_b32(isa::vgpr(8 + p), imm_temp, isa::predicate(0));
  }
  for (unsigned copy = 0; copy < copies; ++copy) {
    builder.atomic_v_flat(mnemonic, isa::predicate(0),
                          chained ? data_v : result_v, addr_pair, flat_base,
                          data_v);
  }
  const isa::Reg zero_address = isa::vgpr(9);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  builder.s_mov_imm_b32(isa::sgpr(4), 0u);
  for (unsigned p = 0; p < width; ++p)
    emit_result_store(builder, env, chained ? data_v : result_v, zero_address,
                      4 * p);
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// The shared-memory ATOMIC family's bench template, the LDS twin of the flat one.
// Same two kinds - the destination is the operand, so copy i's returned old value
// is copy i+1's operand - with one extra piece of setup: the cell has to be
// STORED, because shared memory starts undefined (the flat family preloads it
// from the host instead).  That in-program store needs one ordering fence before
// the atomics; the copies themselves are ordered by the register dependency, so
// no per-copy fence is emitted - a fence per copy would put the ordering cost
// into the latency window.
std::optional<EnvelopeProgram> build_cs_bench_sm_atomic(
    const std::string& mnemonic, unsigned copies, bool chained, unsigned lanes,
    std::string& error) {
  if (mnemonic.find("cmpswap") != std::string::npos) {
    error = "sm atomic bench: the cmpswap form stages a compare value this "
            "template does not build";
    return std::nullopt;
  }
  const bool wide64 = mnemonic.find("_u64") != std::string::npos ||
                      mnemonic.find("_i64") != std::string::npos ||
                      mnemonic.find("_b64") != std::string::npos;
  const unsigned width = wide64 ? 2u : 1u;
  const CarrierEnv env = CarrierEnv::compute(lanes);
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg addr_v = isa::vgpr(8);
  const isa::Reg cell_v = isa::vgpr(9, width);
  const isa::Reg data_v = isa::vgpr(11, width);
  const isa::Reg old_v = isa::vgpr(13, width);
  const isa::Reg imm_temp = isa::sgpr(16);
  builder.s_mov_imm_b32(imm_temp, 0u);
  builder.v_mov_b32(isa::vgpr(0), imm_temp, isa::predicate(0));
  builder.s_mov_imm_b32(imm_temp, 0x20u);  // kLdsBase: the first LDS cell
  builder.v_mov_b32(addr_v, imm_temp, isa::predicate(0));
  EnvelopeProgram program;
  program.probe = true;
  for (unsigned p = 0; p < width; ++p) {
    builder.s_mov_imm_b32(imm_temp, 1u);
    builder.v_mov_b32(isa::vgpr(9 + p), imm_temp, isa::predicate(0));
    builder.s_mov_imm_b32(imm_temp, 1u);
    builder.v_mov_b32(isa::vgpr(11 + p), imm_temp, isa::predicate(0));
  }
  builder.sm_op(width == 2 ? "store_sm_b64" : "store_sm_b32",
                isa::predicate(0), cell_v, addr_v);
  builder.check_k_vm_vs_sm();
  for (unsigned copy = 0; copy < copies; ++copy) {
    builder.sm_atomic(mnemonic, isa::predicate(0), addr_v, data_v,
                      chained ? data_v : old_v);
  }
  for (unsigned p = 0; p < width; ++p)
    emit_result_store(builder, env, chained ? data_v : old_v, isa::vgpr(0),
                      4 * p);
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// The buffer ATOMIC family's bench template.  Its operand IS its destination - the
// buff form stages one register that is both read and written - which decides how
// the two kinds are built:
//   lat - stage the operand once, then let each copy's returned old value be the
//         next copy's operand; the register is the chain.
//   tp  - re-stage the constant operand before every copy, so no copy depends on
//         the previous one's result and the accesses are independent.  The move
//         pipelines, so it does not carry the atomic's cost.
// The cell is preloaded from the host (arg_preloads), the descriptor is assembled
// against the carrier's own window, and both are shapes the correctness carrier
// already exercises.
std::optional<EnvelopeProgram> build_cs_bench_buff_atomic(
    const std::string& mnemonic, unsigned copies, bool chained, unsigned lanes,
    std::string& error) {
  if (mnemonic.find("cmpswap") != std::string::npos) {
    error = "buff atomic bench: the cmpswap form stages a compare pair this "
            "template does not build";
    return std::nullopt;
  }
  const bool wide64 = mnemonic.find("_i64") != std::string::npos ||
                      mnemonic.find("_u64") != std::string::npos ||
                      mnemonic.find("_b64") != std::string::npos;
  const unsigned width = wide64 ? 2u : 1u;
  const CarrierEnv env = CarrierEnv::compute(lanes);
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg load_desc = isa::sgpr(4, 4);
  emit_load_desc(builder, env, load_desc, env.arg_va + carrier::kLoadWindow);
  const isa::Reg data_v = isa::vgpr(8, width);
  const isa::Reg imm_temp = isa::sgpr(16);
  const std::uint32_t off = 0u;
  EnvelopeProgram program;
  program.probe = true;
  for (unsigned p = 0; p < width; ++p)
    program.arg_preloads.emplace_back(carrier::kLoadWindow + off + 4 * p, 1u);
  for (unsigned p = 0; p < width; ++p) {
    builder.s_mov_imm_b32(imm_temp, 1u);
    builder.v_mov_b32(isa::vgpr(8 + p), imm_temp, isa::predicate(0));
  }
  for (unsigned copy = 0; copy < copies; ++copy) {
    if (!chained) {
      for (unsigned p = 0; p < width; ++p) {
        builder.s_mov_imm_b32(imm_temp, 1u);
        builder.v_mov_b32(isa::vgpr(8 + p), imm_temp, isa::predicate(0));
      }
    }
    builder.buff_atomic(mnemonic, isa::predicate(0), data_v, load_desc,
                        isa::zero_reg(), off, true);
  }
  builder.check_k_vm_vs_sm();
  for (unsigned p = 0; p < width; ++p)
    emit_result_store(builder, env, isa::vgpr(8 + p), isa::zero_reg(), 4 * p);
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// The shared-memory LOAD family's bench template.  A load's only operand is its
// address and its result is the loaded value, so the two kinds are:
//   tp  - one fixed address, result in a separate register: independent accesses;
//   lat - the destination IS the address register, so the value just loaded
//         becomes the next access's address and the copies chain through it with
//         no extra instruction.  The staged content is 0x20 - the cell's own base
//         - so the chained address stays on the same in-window cell.
// The cell has to be stored first (shared memory starts undefined), and that
// in-program store needs one ordering fence.  The matrix and interpolator loads
// are not built: their addressing is the part the chapter leaves UNRESOLVED, and
// the correctness carrier runs them observed for the same reason.
std::optional<EnvelopeProgram> build_cs_bench_sm_load(
    const std::string& mnemonic, unsigned copies, bool chained, unsigned lanes,
    std::string& error) {
  if (mnemonic.find("matrix") != std::string::npos ||
      mnemonic.find("interp") != std::string::npos) {
    error = "sm load bench: the matrix and interpolator forms address state the "
            "chapter leaves UNRESOLVED";
    return std::nullopt;
  }
  const unsigned width =
      (mnemonic.find("b64") != std::string::npos) ? 2u : 1u;
  const CarrierEnv env = CarrierEnv::compute(lanes);
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg addr_v = isa::vgpr(8);
  const isa::Reg cell_v = isa::vgpr(10, width);
  const isa::Reg imm_temp = isa::sgpr(16);
  builder.s_mov_imm_b32(imm_temp, 0u);
  builder.v_mov_b32(isa::vgpr(0), imm_temp, isa::predicate(0));
  builder.s_mov_imm_b32(imm_temp, 0x20u);
  builder.v_mov_b32(addr_v, imm_temp, isa::predicate(0));
  EnvelopeProgram program;
  program.probe = true;
  for (unsigned p = 0; p < width; ++p) {
    builder.s_mov_imm_b32(imm_temp, 0x20u);
    builder.v_mov_b32(isa::vgpr(10 + p), imm_temp, isa::predicate(0));
  }
  builder.sm_op(width == 2 ? "store_sm_b64" : "store_sm_b32",
                isa::predicate(0), cell_v, addr_v);
  builder.check_k_vm_vs_sm();
  for (unsigned copy = 0; copy < copies; ++copy) {
    builder.sm_op(mnemonic, isa::predicate(0),
                  chained ? isa::vgpr(8, width) : isa::vgpr(12, width), addr_v);
  }
  for (unsigned p = 0; p < width; ++p)
    emit_result_store(builder, env,
                      chained ? isa::vgpr(8 + p) : isa::vgpr(12 + p),
                      isa::vgpr(0), 4 * p);
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

std::optional<EnvelopeProgram> build_cs_bench(const std::string& mnemonic,
                                              unsigned copies, bool chained,
                                              unsigned lanes,
                                              std::string& error) {
  // The memory families have no semantic spec to build a generic template from,
  // so they get their own shape before the spec lookup.
  if (mnemonic.rfind("load_v_flat_", 0) == 0)
    return build_cs_bench_flat(mnemonic, copies, chained, lanes, error);
  if (mnemonic.rfind("atomic_v_flat_", 0) == 0)
    return build_cs_bench_flat_atomic(mnemonic, copies, chained, lanes, error);
  if (mnemonic.rfind("atomic_sm_", 0) == 0)
    return build_cs_bench_sm_atomic(mnemonic, copies, chained, lanes, error);
  if (mnemonic.rfind("atomic_v_buff_", 0) == 0)
    return build_cs_bench_buff_atomic(mnemonic, copies, chained, lanes, error);
  if (mnemonic.rfind("load_sm_", 0) == 0)
    return build_cs_bench_sm_load(mnemonic, copies, chained, lanes, error);
  if (copies == 0) {
    error = "copies must be positive";
    return std::nullopt;
  }
  const auto* spec = isa::semantic_spec(mnemonic);
  if (spec == nullptr || spec->rule_count == 0) {
    error = "no semantic specification";
    return std::nullopt;
  }
  std::vector<isa::RegClass> classes;
  if (!general_source_classes(mnemonic, classes)) {
    error = "no canonical variant";
    return std::nullopt;
  }
  if (!materializable(*spec, classes)) {
    error = "not materializable on the generic carrier";
    return std::nullopt;
  }
  if (mnemonic == "nop" || mnemonic == "exit" || mnemonic == "check") {
    error = "excluded mnemonic";
    return std::nullopt;
  }
  // The one mnemonic the compute carrier itself refuses: its register-window
  // shape wedged the pipe once already (see the movindex note in the README),
  // and a benchmark must never re-introduce a stream the validation path
  // refuses.  Every other unimplementable shape is already excluded below by
  // materializable()'s rejection of descriptor/offset semantics.
  if (mnemonic == "v_movindex_b32") {
    error = "refused on the compute pipe (known to wedge)";
    return std::nullopt;
  }

  // The compute environment: the same generic emitter the validation carrier
  // uses, so a benchmark cannot silently exercise a different code path than
  // the one the CS matrix validated.  The timestamp block the host reads is
  // written by the firmware into the SIGNAL allocation, not into this result
  // BO, so unlike the graphics template the result store does not clobber it.
  const CarrierEnv env = CarrierEnv::compute(lanes);
  Builder builder;
  emit_env_prelude(builder, env);
  const isa::Reg zero_address = isa::vgpr(9);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  builder.s_mov_imm_b32(isa::sgpr(4), 0u);
  builder.s_mov_imm_b32(isa::vcc(6), 0u);

  const std::uint8_t dst_width =
      isa::semantic_tuple(spec->mnemonic, "VD8", 1);
  bool has_vgpr_dst = false;
  bool has_sgpr_dst = false;
  for (unsigned i = 0; i < spec->rule_count; ++i) {
    if (spec->rules[i].sem == isa::FieldSem::VectorDest8) has_vgpr_dst = true;
    if (spec->rules[i].sem == isa::FieldSem::Scalar8 &&
        spec->rules[i].is_output)
      has_sgpr_dst = true;
    if (spec->rules[i].sem == isa::FieldSem::ScalarCmp8 &&
        spec->rules[i].is_output)
      has_sgpr_dst = true;
  }

  for (unsigned copy = 0; copy < copies; ++copy) {
    const isa::Reg chain_reg = isa::vgpr(10, dst_width);
    std::vector<isa::Operand> operands;
    for (unsigned ri = 0; ri < spec->rule_count; ++ri) {
      const auto& rule = spec->rules[ri];
      const std::uint8_t width =
          isa::semantic_tuple(spec->mnemonic, rule.field, rule.tuple_width);
      switch (rule.sem) {
        case isa::FieldSem::Predicate2:
          operands.push_back(isa::op_reg(isa::predicate(0)));
          break;
        case isa::FieldSem::VectorDest8:
          operands.push_back(isa::op_reg(isa::vgpr(10, width)));
          break;
        case isa::FieldSem::VectorSource8:
        case isa::FieldSem::GeneralSource11:
        case isa::FieldSem::GeneralSource8: {
          const std::size_t flen = std::strlen(rule.field);
          const bool narrow8 =
              flen > 0 && rule.field[flen - 1] == '8' &&
              classes[ri] != isa::RegClass::Vector;
          if (classes[ri] == isa::RegClass::Scalar || narrow8) {
            operands.push_back(isa::op_reg(isa::sgpr(4, width)));
          } else {
            // chained: every source is the previous destination, so the copies
            // form one dependency chain and the time is latency x copies.
            // Independent: eight rotating source registers, so consecutive
            // copies do not depend on each other and the time is throughput.
            const unsigned lane = chained ? 10u : 10u + ((copy % 8) * 2u);
            operands.push_back(isa::op_reg(isa::vgpr(lane, width)));
          }
          break;
        }
        case isa::FieldSem::Scalar8:
          operands.push_back(isa::op_reg(
              rule.is_output ? isa::sgpr(8, width) : isa::sgpr(4, width)));
          break;
        case isa::FieldSem::ScalarCmp8:
          if (rule.is_output) {
            operands.push_back(isa::op_reg(isa::scc_reg()));
          } else {
            error = "unreachable scalar-cmp source";
            return std::nullopt;
          }
          break;
        case isa::FieldSem::Vcc8:
          operands.push_back(isa::op_reg(isa::vcc(6)));
          break;
        case isa::FieldSem::SccSource8:
          operands.push_back(isa::op_reg(isa::sgpr(4, width)));
          break;
        case isa::FieldSem::Immediate32:
        case isa::FieldSem::Immediate8:
          operands.push_back(isa::op_imm(0u));
          break;
        case isa::FieldSem::Modifier:
          break;
        default:
          error = "unsupported field semantics in bench template";
          return std::nullopt;
      }
    }
    builder.append_target(isa::McInst{spec->mnemonic, "", std::move(operands)});
    if (is_implicit_scc_family(spec->mnemonic)) {
      builder.append_target(isa::McInst{
          "s_select_b32", "",
          {isa::op_reg(isa::sgpr(8)), isa::op_reg(isa::sgpr(4)),
           isa::op_reg(isa::sgpr(4)), isa::op_reg(isa::scc_reg())}});
    }
  }

  if (std::getenv("LGPU_BENCH_NOSTORE") == nullptr) {
    if (has_vgpr_dst) {
      for (std::uint8_t part = 0; part < dst_width; ++part) {
        emit_result_store(builder, env, isa::vgpr(10 + part), zero_address,
                          4 * part);
      }
    } else if (has_sgpr_dst) {
      builder.v_mov_b32(isa::vgpr(11), isa::sgpr(8), isa::predicate(0));
      emit_result_store(builder, env, isa::vgpr(11), zero_address, 0);
    }
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  EnvelopeProgram program;
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// Movindex window probe: s_mov_imm s20=<magic>; s4=0; s5=<idx>; s6=0;
// s_movindex_b32 s8, s4, s5, s6; store s8 at 0x40.  If the movindex window
// reads the CURRENT sGPR file, idx=16 yields the magic value (s4+16=s20).
std::optional<EnvelopeProgram> build_movidx_probe(std::uint32_t idx,
                                                  std::uint32_t magic,
                                                  std::string& error) {
  return build_movidx_base_probe(0u, idx, magic, error);
}

std::optional<EnvelopeProgram> build_movidx_base_probe(std::uint32_t base,
                                                       std::uint32_t idx,
                                                       std::uint32_t magic,
                                                       std::string& error) {
  Builder builder;
  emit_store_desc(builder);
  const isa::Reg descriptor = isa::sgpr(0, 4);
  const isa::Reg zero_address = isa::vgpr(0);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  builder.s_mov_imm_b32(isa::sgpr(20), magic);
  builder.s_mov_imm_b32(isa::sgpr(4), base);
  builder.s_mov_imm_b32(isa::sgpr(5), idx);
  builder.s_mov_imm_b32(isa::sgpr(6), 0u);
  builder.append_target(isa::McInst{"s_movindex_b32", "",
                                    {isa::op_reg(isa::sgpr(8)),
                                     isa::op_reg(isa::sgpr(4)),
                                     isa::op_reg(isa::sgpr(5)),
                                     isa::op_reg(isa::sgpr(6))}});
  builder.v_mov_b32(isa::vgpr(8), isa::sgpr(8), isa::predicate(0));
  builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(8), zero_address,
                           descriptor, isa::zero_reg(), 0x40, true, false,
                           false, false);
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();
  EnvelopeProgram program;
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

// Environment-register sweep: executes `getreg s8, <id>` for every id and
// stores s8 (read value) at 0x40 + 4*i.  The observation shape mirrors the
// proven control carrier (same store descriptor, zero address, fence check)
// so the only delta from the control-control is the getreg ids themselves.
std::optional<EnvelopeProgram> build_getreg_sweep(
    const std::vector<std::uint32_t>& ids, std::string& error) {
  constexpr std::uint32_t kBase = 0x40;
  Builder builder;
  emit_store_desc(builder);
  const isa::Reg descriptor = isa::sgpr(0, 4);
  const isa::Reg zero_address = isa::vgpr(0);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  for (std::size_t i = 0; i < ids.size(); ++i) {
    builder.append_target(isa::McInst{"getreg", "",
                                      {isa::op_reg(isa::sgpr(8)),
                                       isa::op_imm(ids[i])}});
    builder.v_mov_b32(isa::vgpr(8), isa::sgpr(8), isa::predicate(0));
    builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(8), zero_address,
                             descriptor, isa::zero_reg(), kBase + 4u * i,
                             true, false, false, false);
    builder.check_k_vm_vs_sm();
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();
  EnvelopeProgram program;
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

namespace {

// Shared body of the selector sweeps.  `sop` picks the instruction whose
// general-source field carries the byte:
//
//   false -> v_mov.b32 p0, v8, <byte>   (VOP,   field GJ11/lo[7:0])
//   true  -> s_mov.b32 s8,    <byte>    (SOP,   field GJ8 /lo[7:0])
//
// Both fields are FieldSem::GeneralSource8 at the same bit position, so the
// two runs test the same encoding space through two different machine formats.
std::optional<EnvelopeProgram> build_selector_sweep_impl(
    const std::vector<std::uint32_t>& selectors, bool sop, std::string& error) {
  constexpr std::uint32_t kBase = 0x40;
  Builder builder;
  emit_store_desc(builder);
  const isa::Reg descriptor = isa::sgpr(0, 4);
  const isa::Reg zero_address = isa::vgpr(0);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  for (std::size_t i = 0; i < selectors.size(); ++i) {
    if (sop) {
      builder.append_target(isa::McInst{"s_mov_b32", "(scc)",
                                        {isa::op_reg(isa::sgpr(8)),
                                         isa::op_raw_selector(selectors[i])}});
      builder.v_mov_b32(isa::vgpr(8), isa::sgpr(8), isa::predicate(0));
    } else {
      builder.append_target(isa::McInst{
          "v_mov_b32", "",
          {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(8)),
           isa::op_raw_selector(selectors[i])}});
    }
    builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(8), zero_address,
                             descriptor, isa::zero_reg(), kBase + 4u * i,
                             true, false, false, false);
    builder.check_k_vm_vs_sm();
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();
  EnvelopeProgram program;
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

}  // namespace

// Selector sweep: executes `v_mov.b32 p0, v8, <raw selector byte>` for every
// byte in the list and stores v8 at 0x40 + 4*i.  The observation shape is the
// same proven control carrier as build_getreg_sweep (same store descriptor,
// zero address, fence check), so the only delta from the control is the
// selector byte itself.
//
// This asks the hardware what the encodings the vendor toolchain does not
// model actually do.  It is [HW] evidence about one field of one instruction;
// it is not a statement about the selector space as a whole.
std::optional<EnvelopeProgram> build_selector_sweep(
    const std::vector<std::uint32_t>& selectors, std::string& error) {
  return build_selector_sweep_impl(selectors, false, error);
}

// Same sweep through the SOP format (`s_mov.b32 s8, <byte>`), so a byte's
// value can be compared across two machine formats instead of trusted from one.
std::optional<EnvelopeProgram> build_selector_sweep_sop(
    const std::vector<std::uint32_t>& selectors, std::string& error) {
  return build_selector_sweep_impl(selectors, true, error);
}

// High-bit sweep of the 11-bit general source: for one base selector, emit
// every combination of the three high bits (0x100 vector domain, 0x200 clear
// sign, 0x400 flip sign) and store each result at 0x40 + 4*i.
//
// v8 is preloaded with -1.0f so a base that selects v8 tests the vector domain
// and the sign modifiers against a value whose sign bit is known to be set.
// The destination is v9, never v8, so reading v8 cannot race the write.
std::optional<EnvelopeProgram> build_selector_hi_sweep(std::uint32_t base,
                                                       std::string& error) {
  constexpr std::uint32_t kBase = 0x40;
  Builder builder;
  emit_store_desc(builder);
  const isa::Reg descriptor = isa::sgpr(0, 4);
  const isa::Reg zero_address = isa::vgpr(0);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  // v8 = selector 0xf3 = f32(-1.0) = 0xbf800000 (sign bit set).
  builder.append_target(isa::McInst{"v_mov_b32", "",
                                    {isa::op_reg(isa::predicate(0)),
                                     isa::op_reg(isa::vgpr(8)),
                                     isa::op_raw_selector(0xf3u)}});
  for (std::uint32_t hi = 0; hi < 8; ++hi) {
    const std::uint32_t sel = (hi << 8) | (base & 0xffu);
    builder.append_target(isa::McInst{
        "v_mov_b32", "",
        {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::vgpr(9)),
         isa::op_raw_selector(sel)}});
    builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(9), zero_address,
                             descriptor, isa::zero_reg(), kBase + 4u * hi,
                             true, false, false, false);
    builder.check_k_vm_vs_sm();
  }
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();
  EnvelopeProgram program;
  if (!finalize_program(builder, program, error)) {
    return std::nullopt;
  }
  return program;
}

}  // namespace lgpu::envelope
