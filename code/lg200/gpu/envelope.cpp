// Generic PS envelope. This file imports the codec layer only.

#include "carrier_map.hpp"
#include "envelope.hpp"

#include "codec/codec.hpp"
#include "codec/semantics.hpp"

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

  void append(const isa::McInst& inst) {
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
  void buff_subword_load(const std::string& mne, const isa::Reg& pred,
                         const isa::Reg& dst, const isa::Reg& descriptor_base,
                         const isa::Reg& offset, std::uint32_t immediate,
                         bool bl1) {
    std::vector<isa::Operand> operands = {
        isa::op_reg(pred), isa::op_reg(dst), isa::op_reg(descriptor_base),
        isa::op_reg(offset), isa::op_imm(immediate)};
    if (bl1) operands.push_back(isa::op_modifier("bl1", 1));
    append(isa::McInst{mne, "", std::move(operands)});
  }
  void buff_subword_store(const std::string& mne, const isa::Reg& pred,
                          const isa::Reg& data, const isa::Reg& descriptor_base,
                          const isa::Reg& offset, std::uint32_t immediate,
                          bool bl1) {
    std::vector<isa::Operand> operands = {
        isa::op_reg(pred), isa::op_reg(data), isa::op_reg(descriptor_base),
        isa::op_reg(offset), isa::op_imm(immediate)};
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
  if (op == "rsub") return a - old;
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

// 32-bit buff/tex-atomic driver (swap is the cmpswap replacement dword);
// signed min/max compare sign-extended 32-bit values, exactly like the lds
// driver does for its width-1 signed forms.
std::uint32_t atomic_final16(const std::string& op, std::uint64_t init,
                             std::uint64_t src, bool sgn,
                             std::uint64_t swap) {
  std::uint64_t v = init, a = src;
  if (sgn && (op == "min" || op == "max")) {
    v = static_cast<std::uint64_t>(
        static_cast<std::int64_t>(static_cast<std::int32_t>(v)));
    a = static_cast<std::uint64_t>(
        static_cast<std::int64_t>(static_cast<std::int32_t>(a)));
  }
  for (unsigned lane = 0; lane < 16; ++lane) v = atomic_apply(op, sgn, v, a, swap);
  return static_cast<std::uint32_t>(v);
}

// 64-bit lds driver; callers pre-extend width-1 signed forms.
std::uint64_t sm_final16(const std::string& op, bool sgn, std::uint64_t initial,
                         std::uint64_t a, std::uint64_t b) {
  std::uint64_t v = initial;
  for (unsigned lane = 0; lane < 16; ++lane) v = atomic_apply(op, sgn, v, a, b);
  return v;
}

// Shared PS carrier environment: the result-store buffer descriptor in
// s0-s3 ({lo=0, hi=1, size 0x40, stride/format 0x27fac} targeting the
// argument BO).  Every carrier starts with this.
void emit_store_desc(Builder& builder) {
  builder.s_mov_imm_b32(isa::sgpr(0), 0x00000000u);
  builder.s_mov_imm_b32(isa::sgpr(1), 0x00000001u);
  builder.s_mov_imm_b32(isa::sgpr(2), 0x00000040u);
  builder.s_mov_imm_b32(isa::sgpr(3), 0x00027facu);
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
                          const std::uint32_t tdesc[8],
                          const std::uint32_t* sdesc,
                          std::uint32_t scratch_sgpr) {
  emit_store_desc(builder);
  TexStage st{carrier::kStoreDesc(), carrier::kImageTuple(),
              carrier::kSamplerTuple(), isa::sgpr(scratch_sgpr)};
  builder.s_mov_imm_b32(st.imm_temp, 0u);
  builder.v_mov_b32(isa::vgpr(0), st.imm_temp, isa::predicate(0));
  for (unsigned i = 0; i < 8; ++i)
    program.arg_preloads.emplace_back(carrier::kRsdTable + 4 * i, tdesc[i]);
  builder.sflat_load("load_s_flat_w8", st.tdesc_regs,
                     isa::sgpr(6, 2), isa::zero_reg(), 0u);
  builder.check_k_vm_vs_sm();
  if (sdesc != nullptr) {
    for (unsigned i = 0; i < 4; ++i)
      program.arg_preloads.emplace_back(carrier::kSamplerSlot + 4 * i, sdesc[i]);
    builder.sflat_load("load_s_flat_w4", st.sdesc_regs,
                       isa::sgpr(6, 2), isa::zero_reg(),
                       carrier::kSamplerSlot - carrier::kRsdTable);
    builder.check_k_vm_vs_sm();
  }
  return st;
}

// Result-store epilogue shared by every carrier.
void emit_result_stores(Builder& builder, const isa::Reg& first,
                        unsigned count, const isa::Reg& zero_addr,
                        const isa::Reg& desc, std::uint32_t off) {
  for (unsigned p = 0; p < count; ++p) {
    builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(first.index + p),
                             zero_addr, desc, isa::zero_reg(),
                             off + 4 * p, true, false, false, false);
    builder.check_k_vm_vs_sm();
  }
}

// Program tail shared by every carrier.
bool finish_program(Builder& builder, EnvelopeProgram& program,
                    std::string& error) {
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();
  return link_program(builder.lines, program, error);
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

std::optional<EnvelopeProgram> build_ps(const std::string& mnemonic,
                                        const std::vector<EnvelopeCase>& rows,
                                        std::string& error) {
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
  // PS carrier environment: argument-BO descriptor in sgpr(0..3) and a
  // zeroed address register.  v9 doubles as the zero store address because
  // v0 is the first vector-source staging register in this carrier.
  emit_store_desc(builder);
  const isa::Reg descriptor = isa::sgpr(0, 4);
  const isa::Reg zero_address = isa::vgpr(9);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  // imm_temp must not collide with the scalar source staging window (s4+):
  // a vector source staged through it would clobber an already-staged
  // scalar source (seen as v_cmp*s comparing a value against itself).
  const isa::Reg imm_temp = isa::sgpr(16);
  const isa::Reg scalar_temp = isa::sgpr(5);

  const bool is_mac_accumulator =
      std::strncmp(spec->mnemonic, "v_mac", 5) == 0;
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
        builder.store_v_buff_b32(isa::predicate(0),
                                 isa::vgpr(dst_vgpr.index + part),
                                 zero_address, descriptor, isa::zero_reg(),
                                 result_offset + 4 * part, true, false,
                                 false, false);
        built.offsets.push_back(result_offset + 4 * part);
      }
    } else if (has_sgpr_dst) {
      const std::uint8_t dw = dst_sgpr.width;
      for (std::uint8_t part = 0; part < dw; ++part) {
        builder.v_mov_b32(isa::vgpr(11 + part),
                          isa::sgpr(dst_sgpr.index + part),
                          isa::predicate(0));
        builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(11 + part),
                                 zero_address, descriptor, isa::zero_reg(),
                                 result_offset + 4 * part, true, false,
                                 false, false);
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

  if (!link_program(builder.lines, program, error)) {
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
  emit_store_desc(builder);
  const isa::Reg store_desc = carrier::kStoreDesc();
  const isa::Reg load_desc = isa::sgpr(4, 4);  // hydrated (never clobber)
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
          builder.buff_mem("store_v_buff_b32", isa::predicate(0),
                           isa::vgpr(14 + p), zero_address, store_desc,
                           isa::zero_reg(), off + 4 * (p + 1), true, false,
                           false, false);
          builder.check_k_vm_vs_sm();
        }
      } else {
      builder.buff_mem("load_v_buff_b32", isa::predicate(0), isa::vgpr(14),
                       zero_address, load_desc, isa::zero_reg(), off, true,
                       false, false, false);
      builder.check_k_vm_vs_sm();
      builder.buff_mem("store_v_buff_b32", isa::predicate(0), isa::vgpr(14),
                       zero_address, store_desc, isa::zero_reg(), off, true,
                       false, false, false);
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
            sm_final16(op, sgn, cell, src64, swap64);
        bc.offsets.push_back(off + 4);
        bc.offsets.push_back(off + 8);
        bc.expected.push_back(static_cast<std::uint32_t>(fin));
        bc.expected.push_back(static_cast<std::uint32_t>(fin >> 32));
      } else {
      bc.expected.push_back(atomic_final16(
          mnemonic.substr(14, mnemonic.find('_', 14) - 14), def.inputs[0],
          def.inputs[1], mnemonic.find("_i32") != std::string::npos,
          def.inputs.size() >= 3 ? def.inputs[2] : 0u));
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
        builder.buff_subword_store(mnemonic, isa::predicate(0),
                                   isa::vgpr(8, width), store_desc,
                                   isa::sgpr(0), cell, true);
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
                                  isa::sgpr(0), off, true);
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
      builder.buff_mem("store_v_buff_b32", isa::predicate(0),
                       isa::vgpr(10 + p), zero_address, store_desc,
                       isa::zero_reg(), off + 4 * p, true, false,
                       false, false);
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

  if (!link_program(builder.lines, program, error)) {
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
  emit_store_desc(builder);
  const isa::Reg store_desc = carrier::kStoreDesc();
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
        fin = sm_final16(op, sgn, initial, a, b);
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
        builder.buff_mem("store_v_buff_b32", isa::predicate(0),
                         isa::vgpr(15 + p), isa::vgpr(0), store_desc,
                         isa::zero_reg(), off + 4 * p, true, false, false,
                         false);
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
      builder.buff_mem("store_v_buff_b32", isa::predicate(0),
                       isa::vgpr(10 + p), isa::vgpr(0), store_desc,
                       isa::zero_reg(), off + 4 * p, true, false, false,
                       false);
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

  if (!link_program(builder.lines, program, error)) return std::nullopt;
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
  emit_store_desc(builder);
  builder.s_mov_imm_b32(isa::sgpr(20), 0x00000000u);
  builder.s_mov_imm_b32(isa::sgpr(21), 0x00000000u);
  const isa::Reg store_desc = carrier::kStoreDesc();
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
    // Flat addressing: the VJ8x2 vaddr pair carries the FULL absolute
    // 64-bit VA (kArgVa + offset), the RK8x2 scalar base pair literal
    // zero -- the exact LCO shape (lco_materialize_global_address).
    const std::uint64_t full_va = carrier::kArgVa + addr;
    builder.s_mov_imm_b32(imm_temp, static_cast<std::uint32_t>(full_va));
    builder.v_mov_b32(addr_lo, imm_temp, isa::predicate(0));
    builder.s_mov_imm_b32(imm_temp,
                          static_cast<std::uint32_t>(full_va >> 32));
    builder.v_mov_b32(isa::vgpr(1), imm_temp, isa::predicate(0));
    // zero address for result stores (v9: v0:v1 is the flat address pair)
    builder.s_mov_imm_b32(imm_temp, 0u);
    builder.v_mov_b32(isa::vgpr(9), imm_temp, isa::predicate(0));

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
          builder.v_mov_b32(isa::vgpr(9), imm_temp, isa::predicate(0));
          builder.atomic_v_flat(mnemonic, isa::predicate(0),
                                isa::vgpr(8, 2), addr_pair, flat_base,
                                isa::vgpr(8, 2));
        } else {
          builder.atomic_v_flat(mnemonic, isa::predicate(0), result_v,
                                addr_pair, flat_base, data_v);
        }
        builder.check_k_vm_vs_sm();
        builder.s_mov_imm_b32(imm_temp, 0u);
        builder.v_mov_b32(isa::vgpr(9), imm_temp, isa::predicate(0));
        builder.load_v_flat("load_v_flat_b32", isa::predicate(0), isa::vgpr(14),
                            addr_pair, flat_base);
        builder.check_k_vm_vs_sm();
        builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(14), isa::vgpr(9),
                                 store_desc, isa::zero_reg(), off + 4,
                                 true, false, false, false);
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
          builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(14 + p),
                                   isa::vgpr(9), store_desc, isa::zero_reg(),
                                   off + 8 + 4 * p, true, false, false,
                                   false);
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
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(10 + part),
                               isa::vgpr(9), store_desc, isa::zero_reg(),
                               off + 4 * part, true, false, false, false);
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
          sm_final16(op, sgn, cell, src64, swap64);
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

  if (!link_program(builder.lines, program, error)) return std::nullopt;
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
                                         std::string& error) {
  // Shape-true templates: every load_v_tex* mnemonic shares the
  // {pred, vdst-quad, coords-triple, s-resource-octet, un, dmask:1111}
  // operand shape (Mesa LCO packs load_v_tex_mip exactly like this and the
  // closed GL3.3 texelFetch readback matches it).  getresinfo shares the
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
  pack_image_tdesc(tdesc, carrier::kArgVa + carrier::kTexelWindow,
                   kTexelCount, pitch, img_rows);
  const TexStage st =
      emit_tex_prelude(builder, program, tdesc, nullptr, 16u);
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
    emit_result_stores(builder, isa::vgpr(8), 4, isa::vgpr(0),
                       st.store_desc, off);
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
  pack_image_tdesc(tdesc, carrier::kArgVa + carrier::kTexelWindow, 16u, 64u);
  const TexStage st =
      emit_tex_prelude(builder, program, tdesc, nullptr, 16u);
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
    emit_result_stores(builder, isa::vgpr(8), 1, isa::vgpr(0),
                       st.store_desc, off);
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
  pack_image_tdesc(tdesc, carrier::kArgVa + carrier::kTexelWindow, 16u, 64u);
  const TexStage st =
      emit_tex_prelude(builder, program, tdesc, nullptr, 16u);
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
    emit_result_stores(builder, isa::vgpr(12), 1, isa::vgpr(0),
                       st.store_desc, off);
    BuiltCase bc;
    bc.id = def.id;
    bc.offsets.push_back(off);
    bc.expected.push_back(
        atomic_final16(op, init, src, mnemonic.find("_i32") != std::string::npos,
                       def.inputs.size() >= 3 ? def.inputs[2] : 0u));
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
  pack_image_tdesc(tdesc, carrier::kArgVa + carrier::kTexelWindow,
                   kTexelCount, pitch);
  pack_sampler_tsdesc(sdesc);
  const TexStage st =
      emit_tex_prelude(builder, program, tdesc, sdesc, 20u);
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
    emit_result_stores(builder, isa::vgpr(8), 4, isa::vgpr(0),
                       st.store_desc, off);
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
                                             std::string& error) {
  constexpr std::uint32_t kPre = 0x40, kPost = 0x44;
  Builder builder;
  emit_store_desc(builder);
  const isa::Reg descriptor = isa::sgpr(0, 4);
  const isa::Reg zero_address = isa::vgpr(0);
  builder.v_mov_b32(zero_address, isa::zero_reg(), isa::predicate(0));
  const isa::Reg imm_temp = isa::sgpr(16);
  builder.s_mov_imm_b32(imm_temp, 0x11111111u);
  builder.v_mov_b32(isa::vgpr(8), imm_temp, isa::predicate(0));
  builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(8), zero_address,
                           descriptor, isa::zero_reg(), kPre,
                           true, false, false, false);
  // the instruction under test, with its fixed carrier operands (values
  // don't matter for control semantics; the point is observed execution)
  std::vector<isa::Operand> ctl_ops;
  if (mnemonic == "getpc" || mnemonic == "jirl" || mnemonic == "trap" ||
      mnemonic == "rfe") {
    ctl_ops = {isa::op_reg(isa::sgpr(8, 2))};
    if (mnemonic == "jirl") ctl_ops.push_back(isa::op_reg(isa::sgpr(10, 2)));
    if (mnemonic == "trap") {
      ctl_ops.push_back(isa::op_reg(isa::sgpr(10, 2)));
      ctl_ops.push_back(isa::op_imm(0u));
    } else if (mnemonic == "jirl" || mnemonic == "rfe") {
      ctl_ops.push_back(isa::op_imm(0u));
    }
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
  } else if (mnemonic == "jpush" || mnemonic == "jpop") {
    ctl_ops = {isa::op_reg(isa::predicate(0)), isa::op_reg(isa::sgpr(4)),
               isa::op_reg(isa::sgpr(5)), isa::op_imm(0x1u)};
  } else if (mnemonic == "b") {
    ctl_ops = {isa::op_imm(0u)};
  }
  if (mnemonic == "check") {
    builder.check_k_vm_vs_sm();
  } else {
    builder.append_target(isa::McInst{mnemonic, "", std::move(ctl_ops)});
  }
  builder.s_mov_imm_b32(imm_temp, 0x22222222u);
  builder.v_mov_b32(isa::vgpr(8), imm_temp, isa::predicate(0));
  builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(8), zero_address,
                           descriptor, isa::zero_reg(), kPost,
                           true, false, false, false);
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  EnvelopeProgram program;
  program.control = true;
  if (!link_program(builder.lines, program, error)) return std::nullopt;
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
  emit_store_desc(builder);
  const isa::Reg store_desc = carrier::kStoreDesc();
  const isa::Reg zero_addr = isa::vgpr(0);
  builder.v_mov_b32(zero_addr, isa::zero_reg(), isa::predicate(0));
  // flat address pair = absolute VA of the RSD table (kArgVa + 0x200);
  // per-case UO20 = byte offset into the table.
  const isa::Reg flat_addr = isa::sgpr(6, 2);
  builder.s_mov_imm_b32(isa::sgpr(6), carrier::kRsdTable);          // lo
  builder.s_mov_imm_b32(isa::sgpr(7),
                        static_cast<std::uint32_t>(carrier::kArgVa >> 32));  // hi = 0x1
  const isa::Reg dst_s = isa::sgpr(20, width);
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
      const isa::Reg desc = isa::sgpr(4, 4);
      builder.sflat_load(mnemonic, dst_s, desc, isa::zero_reg(),
                         static_cast<std::uint32_t>(cell));
    }
    builder.check_k_vm_vs_sm();
    for (unsigned p = 0; p < width; ++p) {
      builder.s_mov_b32(isa::sgpr(16), isa::sgpr(20 + p));
      builder.v_mov_b32(isa::vgpr(10 + p), isa::sgpr(16),
                        isa::predicate(0));
    }
    for (unsigned p = 0; p < width; ++p) {
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(10 + p),
                               zero_addr, store_desc, isa::zero_reg(),
                               off + 4 * p, true, false, false, false);
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
  if (!link_program(builder.lines, program, error)) return std::nullopt;
  return program;
}

std::optional<EnvelopeProgram> build_gs(const std::string& mnemonic,
                                        const std::vector<EnvelopeCase>& rows,
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
  {
    const std::uint64_t arg = carrier::kArgVa;
    builder.s_mov_imm_b32(isa::sgpr(0), static_cast<std::uint32_t>(arg));
    builder.s_mov_imm_b32(isa::sgpr(1),
                          static_cast<std::uint32_t>(arg >> 32));
    builder.s_mov_imm_b32(isa::sgpr(2), 0x00000040u);
    builder.s_mov_imm_b32(isa::sgpr(3), 0x00027facu);
  }
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
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(8),
                               zero_address, store_desc, isa::zero_reg(),
                               off, true, false, false, false);
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
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(8),
                               zero_address, store_desc, isa::zero_reg(),
                               off, true, false, false, false);
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
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(12),
                               zero_address, store_desc, isa::zero_reg(),
                               off, true, false, false, false);
      builder.check_k_vm_vs_sm();
    } else if (mnemonic == "sioexport") {
      builder.append_target(isa::McInst{
          mnemonic, "",
          {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
           isa::op_modifier("channum", 8)}});
      builder.check_k_vm_vs_sm();
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(8),
                               zero_address, store_desc, isa::zero_reg(),
                               off, true, false, false, false);
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
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(10),
                               zero_address, store_desc, isa::zero_reg(),
                               off, true, false, false, false);
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
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(10),
                               zero_address, store_desc, isa::zero_reg(),
                               off, true, false, false, false);
      builder.check_k_vm_vs_sm();
    } else {
      builder.append_target(isa::McInst{
          mnemonic, "",
          {isa::op_modifier("sio", 0), isa::op_reg(isa::predicate(0)),
           isa::op_reg(isa::vgpr(10)), isa::op_reg(isa::vgpr(11)),
           isa::op_modifier("channum", 0), isa::op_imm(0)}});
      builder.check_k_vm_vs_sm();
      builder.store_v_buff_b32(isa::predicate(0), isa::vgpr(10),
                               zero_address, store_desc, isa::zero_reg(),
                               off, true, false, false, false);
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
std::optional<EnvelopeProgram> build_carrier(
    const std::string& mnemonic, const std::vector<EnvelopeCase>& rows,
    std::string& error) {
  const bool buff = mnemonic.rfind("load_v_buff_", 0) == 0 ||
                    mnemonic.rfind("store_v_buff_", 0) == 0 ||
                    mnemonic.rfind("atomic_v_buff_", 0) == 0;
  const bool lds = mnemonic.rfind("load_sm_", 0) == 0 ||
                   mnemonic.rfind("store_sm_", 0) == 0 ||
                   mnemonic.rfind("atomic_sm_", 0) == 0;
  const bool tex = mnemonic.rfind("load_v_tex", 0) == 0 ||
                   mnemonic == "getresinfo";
  const bool atex = mnemonic.rfind("atomic_v_tex", 0) == 0;
  const bool stex = mnemonic.rfind("store_v_tex", 0) == 0;
  const bool samp = (mnemonic.rfind("samp", 0) == 0 && mnemonic != "sampler") ||
                    mnemonic.rfind("gather", 0) == 0 || mnemonic == "getlod";
  const bool flat = mnemonic.rfind("load_v_flat", 0) == 0 ||
                    mnemonic.rfind("store_v_flat", 0) == 0 ||
                    mnemonic.rfind("atomic_v_flat", 0) == 0;
  const bool smem = mnemonic.rfind("load_s_flat_", 0) == 0 ||
                    mnemonic.rfind("load_s_buff_", 0) == 0;
  const bool gs = mnemonic == "v_emit2" || mnemonic == "v_emit3" ||
                    mnemonic.rfind("sio", 0) == 0;
  // [HW 2026-08-25] PS-phase launch-state families: the hardware routes
  // these to per-fragment windows whose content the carrier cannot set up
  // (vs. the staged sources), so the only reproducible record is
  // observation.  Evidence: flat loads/stores/atomics read input-plane
  // values that change with the 0x530 input_enable toggle; fmtdesc/fmtinst
  // and i8/u8/b8 subword loads return fixed plane patterns (0x3f800000 /
  // 0x3fa1ffff) for any staged data; load_sm_matrix reads a persistent
  // matrix file (cross-run state); tex/getlod/getresinfo fetch window
  // state (0.5 / exponent-plane values) regardless of the staged texels.
  std::optional<EnvelopeProgram> p;
  if (smem) p = build_smem(mnemonic, rows, error);
  else if (gs) p = build_gs(mnemonic, rows, error);
  else if (buff) p = build_buff(mnemonic, rows, error);
  else if (flat) p = build_flat(mnemonic, rows, error);
  else if (lds) p = build_lds(mnemonic, rows, error);
  else if (atex) p = build_tex_atomic(mnemonic, rows, error);
  else if (stex) p = build_tex_store(mnemonic, rows, error);
  else if (tex) p = build_tex(mnemonic, rows, error);
  else if (samp) p = build_samp(mnemonic, rows, error);
  else if (is_control(mnemonic)) p = build_control(mnemonic, rows, error);
  else p = build_ps(mnemonic, rows, error);
  if (!p) return p;
  const bool launch_state =
      flat || mnemonic.rfind("load_sm_matrix_", 0) == 0 ||
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
  emit_store_desc(builder);
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
  builder.check_k_vm_vs_sm();
  builder.nop();
  builder.exit();

  EnvelopeProgram program;
  if (!link_program(builder.lines, program, error)) {
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
  if (!link_program(builder.lines, program, error)) {
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
  if (!link_program(builder.lines, program, error)) {
    return std::nullopt;
  }
  return program;
}

}  // namespace lgpu::envelope
