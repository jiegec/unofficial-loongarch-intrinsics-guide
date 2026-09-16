#include "codec.hpp"

#include "generated/lg200_codec.inc"
#include "semantics.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

namespace lgpu::isa {
namespace {

constexpr std::uint32_t kFieldMask(std::uint8_t bits) {
  return bits >= 32 ? 0xffffffffu : ((1u << bits) - 1u);
}

const generated::CodecRow* find_generated_row(const std::string& mnemonic) {
  for (const auto& row : generated::kCodecRows) {
    if (mnemonic == row.mnemonic) {
      return &row;
    }
  }
  return nullptr;
}

const generated::CodecRow* find_generated_row_by_words(std::uint32_t lo, std::uint32_t hi) {
  const generated::CodecRow* best = nullptr;
  unsigned best_bits = 0;
  for (const auto& row : generated::kCodecRows) {
    if ((lo & row.fixed_lo) != (row.inst_lo & row.fixed_lo) ||
        (hi & row.fixed_hi) != (row.inst_hi & row.fixed_hi)) {
      continue;
    }
    // Prefer the most specific pattern when encodings overlap (e.g. the
    // sio-id family vs vop rows): pick the row with the most fixed bits.
    const unsigned bits = static_cast<unsigned>(
        __builtin_popcount(row.fixed_lo) + __builtin_popcount(row.fixed_hi));
    if (bits > best_bits) {
      best_bits = bits;
      best = &row;
    }
  }
  if (best != nullptr &&
      (std::strcmp(best->mnemonic, "v_sub_imm_f32") == 0 ||
       std::strncmp(best->mnemonic, "load_sm_", 8) == 0 ||
       std::strncmp(best->mnemonic, "atomic_sm_", 10) == 0 ||
       std::strncmp(best->mnemonic, "load_v_flat_", 12) == 0 ||
       std::strncmp(best->mnemonic, "store_v_flat_", 13) == 0)) {
    // v_sub_imm_f32's fixed mask (0xaffc0000) has holes (bit30 is not
    // checked), so sio-id encodings in the 0x108xxxxx zone (bit30=0) can
    // match it.  Check the siowrite row explicitly; siowrite's fixed zone
    // has bit30=0 while v_sub_imm_f32 lives in the 0x50xxxxxx zone
    // (bit30=1), so the checks do not overlap.
    for (const auto& row : generated::kCodecRows) {
      const bool flat_row =
          std::strncmp(row.mnemonic, "load_v_flat_", 12) == 0 ||
          std::strncmp(row.mnemonic, "store_v_flat_", 13) == 0;
      if (std::strcmp(row.mnemonic, "siowrite") != 0 && !flat_row) {
        continue;
      }
      if ((lo & row.fixed_lo) == (row.inst_lo & row.fixed_lo) &&
          (hi & row.fixed_hi) == (row.inst_hi & row.fixed_hi)) {
        return &row;
      }
    }
  }
  return best;
}

int field_index(const generated::CodecRow& row, const char* name) {
  for (unsigned i = 0; i < row.field_count; ++i) {
    if (std::strcmp(generated::kCodecFields[row.field_start + i].name, name) == 0) {
      return static_cast<int>(i);
    }
  }
  // Tuple variants (VD8 matches VD8x2/VD8x3/VD8x4) resolve by prefix.
  const std::size_t length = std::strlen(name);
  for (unsigned i = 0; i < row.field_count; ++i) {
    const char* field = generated::kCodecFields[row.field_start + i].name;
    if (std::strncmp(field, name, length) == 0 && field[length] == 'x') {
      return static_cast<int>(i);
    }
  }
  return -1;
}

std::uint32_t extract_field(const generated::CodecRow& row, int index,
                            std::uint32_t lo, std::uint32_t hi) {
  const auto& field = generated::kCodecFields[row.field_start + index];
  const std::uint32_t lo_part =
      (lo >> field.lo_shift) & kFieldMask(field.lo_bits);
  const std::uint32_t hi_part =
      (hi >> field.hi_shift) & kFieldMask(field.hi_bits);
  return lo_part | (hi_part << field.lo_bits);
}

void insert_field(const generated::CodecRow& row, int index, std::uint32_t value,
                  std::uint32_t& lo, std::uint32_t& hi) {
  const auto& field = generated::kCodecFields[row.field_start + index];
  const std::uint32_t lo_mask = kFieldMask(field.lo_bits);
  lo &= ~(lo_mask << field.lo_shift);
  lo |= ((value & lo_mask) << field.lo_shift);
  const std::uint32_t hi_mask = kFieldMask(field.hi_bits);
  hi &= ~(hi_mask << field.hi_shift);
  hi |= (((value >> field.lo_bits) & hi_mask) << field.hi_shift);
}

// Inline constant encodings defined by the canonical forms:
// i(n) = 0x9F + n for n in 1..41, i(-1) = 0xE0,
// f(+2^k) = 0xF2 + 2k, f(-2^k) = 0xF3 + 2k for k in 0..1. Other values in
// Other values in 0x80-0xFF are reserved and rejected.
bool inline_encode(const Operand& operand, std::uint32_t& value) {
  // Hardware map (2026-07-30 PS carrier, g11-selector-map): i(1)..i(64) at
  // 0xa0..0xdf, i(-1)..i(-16) at 0xe0..0xef, floats at 0xf0..0xfa.
  if (operand.kind == OperandKind::InlineInt) {
    const int32_t n = static_cast<int32_t>(operand.value);
    if (n >= 1 && n <= 64) {
      value = 0x9fu + static_cast<std::uint32_t>(n);
      return true;
    }
    if (n >= -16 && n <= -1) {
      value = 0xdfu + static_cast<std::uint32_t>(-n);
      return true;
    }
    return false;
  }
  if (operand.kind == OperandKind::InlineFloat) {
    static constexpr std::uint32_t kFloat[11] = {
        0x3f000000u,  // 0.5
        0xbf000000u,  // -0.5
        0x3f800000u,  // 1.0
        0xbf800000u,  // -1.0
        0x40000000u,  // 2.0
        0xc0000000u,  // -2.0
        0x40800000u,  // 4.0
        0xc0800000u,  // -4.0
        0x40490fdbu,  // pi
        0x402df854u,  // e
        0x3e22f983u,  // 1/(2*pi)
    };
    for (unsigned k = 0; k < 11; ++k) {
      if (operand.value == kFloat[k]) {
        value = 0xf0u + k;
        return true;
      }
    }
  }
  return false;
}


bool inline_decode(std::uint32_t value, Operand& out) {
  if (value >= 0xa0u && value <= 0xdfu) {
    out = op_inline_int(static_cast<std::uint32_t>(value - 0x9fu));
    return true;
  }
  if (value >= 0xe0u && value <= 0xefu) {
    out = op_inline_int(0x100000000u - static_cast<std::uint32_t>(value - 0xdfu));
    return true;
  }
  static constexpr std::uint32_t kFloat[11] = {
      0x3f000000u,  // 0.5
      0xbf000000u,  // -0.5
      0x3f800000u,  // 1.0
      0xbf800000u,  // -1.0
      0x40000000u,  // 2.0
      0xc0000000u,  // -2.0
      0x40800000u,  // 4.0
      0xc0800000u,  // -4.0
      0x40490fdbu,  // pi
      0x402df854u,  // e
      0x3e22f983u,  // 1/(2*pi)
  };
  for (unsigned k = 0; k < 11; ++k) {
    if (value == 0xf0u + k) {
      out = op_inline_float(kFloat[k]);
      return true;
    }
  }
  return false;
}


bool encode_field_value(const FieldRule& rule, std::uint8_t tuple_width,
                        const Operand& operand, std::uint32_t& value,
                        std::string& error) {
  const auto fail = [&](const char* why) {
    error = std::string("operand for field ") + rule.field + ": " + why;
    return false;
  };
  switch (rule.sem) {
    case FieldSem::Predicate2:
      if (operand.kind != OperandKind::Reg ||
          operand.reg.cls != RegClass::Predicate) {
        return fail("expected predicate p0-p3");
      }
      if (operand.reg.index > 3) {
        return fail("predicate index out of range");
      }
      value = operand.reg.index;
      return true;
    case FieldSem::VectorDest8:
    case FieldSem::VectorSource8:
      if (operand.kind != OperandKind::Reg) {
        return fail("expected vector register");
      }
      switch (operand.reg.cls) {
        case RegClass::Vector:
          if (operand.reg.index > 255 - tuple_width + 1) {
            return fail("vector register tuple out of 8-bit range");
          }
          value = operand.reg.index;
          return true;
        case RegClass::SpecialZero:
          value = 102;
          return true;
        default:
          return fail("register class not allowed in vector field");
      }
    case FieldSem::GeneralSource11:
    case FieldSem::GeneralSource8:
      if (operand.kind == OperandKind::InlineInt ||
          operand.kind == OperandKind::InlineFloat) {
        if (!inline_encode(operand, value)) {
          return fail("reserved inline constant encoding");
        }
        return true;
      }
      if (operand.kind != OperandKind::Reg) {
        return fail("expected register or inline constant source");
      }
      switch (operand.reg.cls) {
        case RegClass::Vector:
          if (operand.reg.index > 255 - tuple_width + 1) {
            return fail("vector register tuple out of source range");
          }
          value = 256u + operand.reg.index;
          return true;
        case RegClass::Scalar:
          if (operand.reg.index > 101) {
            return fail("scalar source index out of range "
                        "(inline-constant space reserved)");
          }
          value = operand.reg.index;
          return true;
        case RegClass::SpecialZero:
          value = 102;
          return true;
        default:
          return fail("register class not allowed in general source");
      }
    case FieldSem::SccSource8:
      // s_select_b32 selector source: scalar register, SCC (0x80), zero,
      // or inline constant (GeneralSource encoding space).
      if (operand.kind == OperandKind::InlineInt ||
          operand.kind == OperandKind::InlineFloat) {
        if (!inline_encode(operand, value)) {
          return fail("reserved inline constant encoding");
        }
        return true;
      }
      if (operand.kind != OperandKind::Reg) {
        return fail("expected register or inline constant source");
      }
      switch (operand.reg.cls) {
        case RegClass::Scalar:
          if (operand.reg.index > 101) {
            return fail("scalar source index out of range");
          }
          value = operand.reg.index;
          return true;
        case RegClass::Scc:
          value = 0x80;
          return true;
        case RegClass::SpecialZero:
          value = 102;
          return true;
        default:
          return fail("register class not allowed in SCC source field");
      }
    case FieldSem::Scalar8:
      if (operand.kind != OperandKind::Reg) {
        return fail("expected scalar register");
      }
      switch (operand.reg.cls) {
        case RegClass::Scalar:
          if (operand.reg.index > 255 - tuple_width + 1) {
            return fail("scalar register tuple out of 8-bit range");
          }
          value = operand.reg.index;
          return true;
        case RegClass::Vcc:
          value = operand.reg.index;
          return true;
        case RegClass::Predicate:
          if (operand.reg.index > 3) {
            return fail("predicate index out of range in 8-bit scalar field");
          }
          value = 0x7c + operand.reg.index;
          return true;
        case RegClass::SpecialZero:
          value = 102;
          return true;
        default:
          return fail("register class not allowed in 8-bit scalar field");
      }
    case FieldSem::ScalarCmp8:
      if (operand.kind != OperandKind::Reg) {
        return fail("expected scalar register or SCC");
      }
      switch (operand.reg.cls) {
        case RegClass::Scc:
          value = 0x80;
          return true;
        case RegClass::Scalar:
          if (operand.reg.index > 127) {
            return fail("scalar index out of range (0x80 reserved for SCC)");
          }
          value = operand.reg.index;
          return true;
        case RegClass::SpecialZero:
          value = 102;
          return true;
        default:
          return fail("register class not allowed in scalar compare field");
      }
    case FieldSem::ScalarQuad8:
      if (operand.kind != OperandKind::Reg ||
          operand.reg.cls != RegClass::Scalar) {
        return fail("expected scalar descriptor base");
      }
      value = operand.reg.index;
      return true;
    case FieldSem::OffsetSource8:
      if (operand.kind != OperandKind::Reg) {
        return fail("expected offset source register");
      }
      switch (operand.reg.cls) {
        case RegClass::SpecialZero:
          value = 102;
          return true;
        case RegClass::Scalar:
        case RegClass::Vector:
          value = operand.reg.index;
          return true;
        default:
          return fail("register class not allowed in offset source");
      }
    case FieldSem::Vcc8:
      if (operand.kind != OperandKind::Reg) {
        return fail("expected vcc register");
      }
      if (operand.reg.cls == RegClass::Vcc) {
        value = operand.reg.index;
        return true;
      }
      if (operand.reg.cls == RegClass::SpecialZero) {
        value = 102;
        return true;
      }
      return fail("vcc field requires vcc(sN)");
    case FieldSem::Immediate32:
      if (operand.kind != OperandKind::Immediate) {
        return fail("expected 32-bit immediate");
      }
      value = operand.value;
      return true;
    case FieldSem::Immediate40:
      if (operand.kind != OperandKind::Immediate) {
        return fail("expected 40-bit immediate");
      }
      if (operand.value > 0xffffffffffull) {
        return fail("40-bit immediate out of range");
      }
      value = static_cast<std::uint32_t>(operand.value);
      return true;
    case FieldSem::SioId:
    case FieldSem::SioChannum:
    case FieldSem::SioChan:
      if (operand.kind == OperandKind::Immediate) {
        value = operand.value;
        return true;
      }
      if (operand.kind == OperandKind::Modifier &&
          (operand.text == "sio" || operand.text == "channum" ||
           operand.text == "chan")) {
        value = operand.value;
        return true;
      }
      return fail("expected SIO slot operand");
    case FieldSem::Immediate16:
      if (operand.kind != OperandKind::Immediate &&
          operand.kind != OperandKind::ShortImmediate) {
        return fail("expected 16-bit immediate");
      }
      if (operand.value > 0xffffu) {
        return fail("16-bit immediate out of range");
      }
      value = operand.value;
      return true;
    case FieldSem::Immediate20:
      if (operand.kind != OperandKind::Immediate &&
          operand.kind != OperandKind::Immediate20) {
        return fail("expected 20-bit immediate");
      }
      if (operand.value > 0xfffffu) {
        return fail("20-bit immediate out of range");
      }
      value = operand.value;
      return true;
    case FieldSem::Immediate23:
      if (operand.kind != OperandKind::Immediate &&
          operand.kind != OperandKind::ShortImmediate) {
        return fail("expected 23-bit immediate");
      }
      if (operand.value > 0x7fffffu) {
        return fail("23-bit immediate out of range");
      }
      value = operand.value;
      return true;
    case FieldSem::Immediate12:
      if (operand.kind != OperandKind::Immediate &&
          operand.kind != OperandKind::ShortImmediate) {
        return fail("expected 12-bit immediate");
      }
      if (operand.value > 0xfffu) {
        return fail("12-bit immediate out of range");
      }
      value = operand.value;
      return true;
    case FieldSem::Immediate8:
      if (operand.kind != OperandKind::Immediate &&
          operand.kind != OperandKind::ShortImmediate) {
        return fail("expected 8-bit immediate");
      }
      if (operand.value > 0xffu) {
        return fail("8-bit immediate out of range");
      }
      value = operand.value;
      return true;
    case FieldSem::Modifier:
      if (operand.kind == OperandKind::Modifier) {
        if (operand.text != rule.modifier_name) {
          return fail("unexpected modifier");
        }
        value = operand.value;
      } else {
        value = 0;
      }
      return true;
  }
  return fail("unhandled field semantics");
}

Operand decode_field_value(const FieldRule& rule, std::uint8_t tuple_width,
                           std::uint32_t value) {
  switch (rule.sem) {
    case FieldSem::Predicate2:
      return op_reg(predicate(static_cast<std::uint16_t>(value)));
    case FieldSem::VectorDest8:
    case FieldSem::VectorSource8:
      if (value == 102) {
        // zero selector: omitted on print (symmetric to encode omission;
        // Buffer atomic forms encode VQ8=0x66 without printing it.
        return Operand{OperandKind::Raw, {}, 0, "", ""};
      }
      return op_reg(vgpr(static_cast<std::uint16_t>(value), tuple_width));
    case FieldSem::GeneralSource11:
    case FieldSem::GeneralSource8:
      if (value >= 0x80u && value <= 0xffu) {
        Operand inline_out;
        if (inline_decode(value, inline_out)) {
          return inline_out;
        }
        return op_raw("<inline-reserved>");
      }
      if (value >= 256 && value < 512) {
        return op_reg(vgpr(static_cast<std::uint16_t>(value - 256), tuple_width));
      }
      if (value == 102) {
        return op_reg(zero_reg());
      }
      if (value > 102) {
        return op_raw("<reserved-encoding>");
      }
      return op_reg(sgpr(static_cast<std::uint16_t>(value), tuple_width));
    case FieldSem::Scalar8:
      if (value == 102) {
        return op_reg(zero_reg());
      }
      return op_reg(sgpr(static_cast<std::uint16_t>(value), tuple_width));
    case FieldSem::ScalarCmp8:
      if (value == 0x80) {
        return op_reg(scc_reg());
      }
      if (value == 102) {
        return op_reg(zero_reg());
      }
      if (value > 0x80) {
        return op_raw("<reserved-encoding>");
      }
      return op_reg(sgpr(static_cast<std::uint16_t>(value), tuple_width));
    case FieldSem::SccSource8:
      if (value == 0x80) {
        return op_reg(scc_reg());
      }
      if (value == 102) {
        return op_reg(zero_reg());
      }
      if (value >= 0x80u && value <= 0xffu) {
        Operand inline_out;
        if (inline_decode(value, inline_out)) {
          return inline_out;
        }
        return op_raw("<inline-reserved>");
      }
      return op_reg(sgpr(static_cast<std::uint16_t>(value), tuple_width));
    case FieldSem::ScalarQuad8:
      return op_reg(sgpr(static_cast<std::uint16_t>(value), 4));
    case FieldSem::OffsetSource8:
      if (value == 102) {
        return op_reg(zero_reg());
      }
      if (value >= 0x80u && value <= 0xffu) {
        Operand inline_out;
        if (inline_decode(value, inline_out)) {
          return inline_out;
        }
        return op_raw("<inline-reserved>");
      }
      return op_reg(sgpr(static_cast<std::uint16_t>(value)));
    case FieldSem::Vcc8:
      return op_reg(vcc(static_cast<std::uint16_t>(value)));
    case FieldSem::Immediate32:
      return op_imm(value);
    case FieldSem::Immediate40:
      // extract_field already merged LO[31:0]+HI[7:0]; the operand value
      // keeps the low 32 bits (signed 40-bit negative offsets lose the
      // sign extension in the operand view - acceptable for positive
      // branch offsets).
      return op_imm(value);
    case FieldSem::SioId:
      return op_modifier("sio", value);
    case FieldSem::SioChannum:
      return op_modifier("channum", value);
    case FieldSem::SioChan:
      return op_modifier("chan", value);
    case FieldSem::Immediate16:
      return op_imm16(value);
    case FieldSem::Immediate12:
      return op_short_imm(value);
    case FieldSem::Immediate20:
      return op_imm20(value);
    case FieldSem::Immediate23:
      return op_imm(value);
    case FieldSem::Immediate8:
      return op_imm8(value);
    case FieldSem::Modifier:
      if (value == 0 && rule.omit_zero) {
        return Operand{OperandKind::Raw, {}, 0, "", ""};
      }
      return op_modifier(rule.modifier_name, value);
  }
  return op_raw("<invalid>");
}

std::string format_instruction(const SemanticSpec& spec,
                               const std::vector<Operand>& operands) {
  std::string out = spec.mnemonic;
  if (spec.asm_prefix && *spec.asm_prefix) {
    out += " ";
    out += spec.asm_prefix;
  }
  std::string operand_text;
  bool printed = false;
  for (const auto& operand : operands) {
    if (operand.kind == OperandKind::Raw && operand.text.empty()) {
      continue;
    }
    operand_text += (printed ? ", " : "");
    operand_text += format_operand(operand);
    printed = true;
  }
  if (!operand_text.empty()) {
    if (!out.empty() && out.back() != ' ') {
      out += " ";
    }
    out += operand_text;
  }
  if (std::strcmp(spec.mnemonic, "s_addc_imm_u32") == 0 ||
      std::strcmp(spec.mnemonic, "s_subb_imm_u32") == 0 ||
      std::strcmp(spec.mnemonic, "s_subbrev_imm_u32") == 0) {
    out += ", SCC";
  }
  return out;
}

bool encode_generic(const generated::CodecRow& row, const SemanticSpec& spec,
                    const std::vector<Operand>& operands, EncodedInst& out,
                    std::string& error) {
  std::uint32_t lo = row.template_lo;
  std::uint32_t hi = row.template_hi;
  std::size_t operand_index = 0;
  for (unsigned rule_index = 0; rule_index < spec.rule_count; ++rule_index) {
    const auto& rule = spec.rules[rule_index];
    const int index = field_index(row, rule.field);
    if (index < 0) {
      error = std::string("missing field in layout: ") + rule.field;
      return false;
    }
    std::uint32_t value = 0;
    if (rule.sem == FieldSem::Modifier) {
      if (operand_index < operands.size() &&
          operands[operand_index].kind == OperandKind::Modifier &&
          operands[operand_index].text == rule.modifier_name) {
        if (!encode_field_value(rule, 1, operands[operand_index], value, error)) {
          return false;
        }
        ++operand_index;
      }
    } else {
      if (operand_index >= operands.size()) {
        error = std::string("missing operand for field ") + rule.field;
        return false;
      }
      const auto& op = operands[operand_index];
      const bool is_vec_field =
          rule.sem == FieldSem::VectorSource8 || rule.sem == FieldSem::VectorDest8;
      const bool op_is_vector =
          op.kind == OperandKind::Reg && op.reg.cls == RegClass::Vector;
      // Vector source/dest field receives a non-vector operand: treat the
      // field as omitted (zero encoding 102) and let the next rule consume
      // the operand.  Symmetric to the disassembler's zero-omission print
      // Buffer atomic forms omit VQ8 from text while encoding 0x66 = zero.
      if (is_vec_field && !op_is_vector && !rule.is_output) {
        std::string zmsg;
        if (!encode_field_value(rule, 1, op_reg(zero_reg()), value, zmsg)) {
          error = zmsg;
          return false;
        }
        insert_field(row, index, value, lo, hi);
        continue;
      }
      const std::uint8_t tuple =
          semantic_tuple(row.mnemonic, rule.field, rule.tuple_width);
      if (!encode_field_value(rule, tuple, op, value, error)) {
        return false;
      }
      ++operand_index;
    }
    insert_field(row, index, value, lo, hi);
  }
  if (operand_index != operands.size()) {
    error = std::string("too many operands for ") + row.mnemonic;
    return false;
  }
  if ((lo & row.fixed_lo) != (row.inst_lo & row.fixed_lo) ||
      (hi & row.fixed_hi) != (row.inst_hi & row.fixed_hi)) {
    error = "encoded instruction violates fixed mask";
    return false;
  }
  out = EncodedInst{lo, hi};
  return true;
}

bool decode_generic(const generated::CodecRow& row, const SemanticSpec& spec,
                    std::uint32_t lo, std::uint32_t hi, DecodedInst& out,
                    std::string& error) {
  std::vector<Operand> operands;
  for (unsigned rule_index = 0; rule_index < spec.rule_count; ++rule_index) {
    const auto& rule = spec.rules[rule_index];
    const int index = field_index(row, rule.field);
    if (index < 0) {
      error = std::string("missing field in layout: ") + rule.field;
      return false;
    }
    const std::uint32_t value = extract_field(row, index, lo, hi);
    const std::uint8_t tuple =
        semantic_tuple(row.mnemonic, rule.field, rule.tuple_width);
    Operand operand = decode_field_value(rule, tuple, value);
    if (rule.sem == FieldSem::Modifier && value == 0 && rule.omit_zero) {
      continue;
    }
    operands.push_back(std::move(operand));
  }
  out.mnemonic = row.mnemonic;
  out.operands = std::move(operands);
  out.assembly = format_instruction(spec, out.operands);
  return true;
}

bool encode_sio_export(const std::vector<Operand>& operands, EncodedInst& out,
                       std::string& error) {
  const auto* row = find_generated_row("sioexport");
  if (!row) {
    error = "missing sioexport row";
    return false;
  }
  std::uint32_t lo = row->template_lo;
  std::uint32_t hi = row->template_hi;
  auto set = [&](const char* name, std::uint32_t value) {
    const int index = field_index(*row, name);
    insert_field(*row, index, value, lo, hi);
  };
  set("M.sioid1", 0);
  set("PJ2", 0);
  set("M.channum12", 0);
  for (const auto& operand : operands) {
    if (operand.kind == OperandKind::Modifier && operand.text == "sio") {
      set("M.sioid1", operand.value);
    } else if (operand.kind == OperandKind::Modifier && operand.text == "channum") {
      set("M.channum12", operand.value);
    } else if (operand.kind == OperandKind::Reg &&
               operand.reg.cls == RegClass::Predicate) {
      set("PJ2", operand.reg.index);
    } else {
      error = std::string("unsupported sioexport operand: ") + format_operand(operand);
      return false;
    }
  }
  out = EncodedInst{lo, hi};
  return true;
}

bool decode_sio_export(std::uint32_t lo, std::uint32_t hi, DecodedInst& out,
                       std::string& error) {
  const auto* row = find_generated_row("sioexport");
  if (!row) {
    error = "missing sioexport row";
    return false;
  }
  const std::uint32_t sioid = extract_field(*row, field_index(*row, "M.sioid1"), lo, hi);
  const std::uint32_t pred = extract_field(*row, field_index(*row, "PJ2"), lo, hi);
  const std::uint32_t channum =
      extract_field(*row, field_index(*row, "M.channum12"), lo, hi);
  char buffer[64];
  std::snprintf(buffer, sizeof(buffer), "sioexport SIO%u, p%u, channum(%u)",
                sioid, pred, channum);
  out.mnemonic = "sioexport";
  out.operands = {op_modifier("sio", sioid), op_reg(predicate(static_cast<std::uint16_t>(pred))),
                  op_modifier("channum", channum)};
  out.assembly = buffer;
  return true;
}

bool encode_siowrrot_imm(const std::vector<Operand>& operands, EncodedInst& out,
                         std::string& error) {
  const auto* row = find_generated_row("siowrrot_imm");
  if (!row) {
    error = "missing siowrrot_imm row";
    return false;
  }
  std::uint32_t lo = row->template_lo;
  std::uint32_t hi = row->template_hi;
  auto set = [&](const char* name, std::uint32_t value) {
    const int index = field_index(*row, name);
    insert_field(*row, index, value, lo, hi);
  };
  set("M.sioid1", 1);
  set("PJ2", 0);
  set("VJ8", 0);
  set("M.channum12", 0);
  set("IK16", 0);
  set("M.chan12", 0);
  for (const auto& operand : operands) {
    if (operand.kind == OperandKind::Modifier && operand.text == "sio") {
      set("M.sioid1", operand.value);
    } else if (operand.kind == OperandKind::Reg && operand.reg.cls == RegClass::Predicate) {
      set("PJ2", operand.reg.index);
    } else if (operand.kind == OperandKind::Reg && operand.reg.cls == RegClass::Vector) {
      set("VJ8", operand.reg.index);
    } else if (operand.kind == OperandKind::Modifier && operand.text == "channum") {
      set("M.channum12", operand.value);
    } else if (operand.kind == OperandKind::Modifier && operand.text == "chan") {
      set("M.chan12", operand.value);
    } else if (operand.kind == OperandKind::Immediate) {
      set("IK16", operand.value & 0xffffu);
    } else {
      error = std::string("unsupported siowrrot_imm operand: ") + format_operand(operand);
      return false;
    }
  }
  out = EncodedInst{lo, hi};
  return true;
}

bool decode_siowrrot_imm(std::uint32_t lo, std::uint32_t hi, DecodedInst& out,
                         std::string& error) {
  const auto* row = find_generated_row("siowrrot_imm");
  if (!row) {
    error = "missing siowrrot_imm row";
    return false;
  }
  auto get = [&](const char* name) {
    return extract_field(*row, field_index(*row, name), lo, hi);
  };
  const std::uint32_t sioid = get("M.sioid1");
  const std::uint32_t pred = get("PJ2");
  const std::uint32_t vreg = get("VJ8");
  const std::uint32_t channum = get("M.channum12");
  const std::uint32_t imm = get("IK16");
  const std::uint32_t chan = get("M.chan12");
  char buffer[96];
  std::snprintf(buffer, sizeof(buffer),
                "siowrrot_imm SIO%u, p%u, v%u, channum(%u), 0x%04x, chan(%u)",
                sioid, pred, vreg, channum, imm, chan);
  out.mnemonic = "siowrrot_imm";
  out.operands = {op_modifier("sio", sioid), op_reg(predicate(static_cast<std::uint16_t>(pred))),
                  op_reg(vgpr(static_cast<std::uint16_t>(vreg))),
                  op_modifier("channum", channum), op_imm(imm),
                  op_modifier("chan", chan)};
  out.assembly = buffer;
  return true;
}

}  // namespace

std::vector<ExactVariant> codec_variants() {
  std::vector<ExactVariant> out;
  for (const auto& row : generated::kCanonicalVariants) {
    out.push_back(ExactVariant{
        row.variant_id,
        row.mnemonic,
        row.assembly,  // generated canonical form already
        {row.inst_lo, row.inst_hi},
    });
  }
  return out;
}

bool encode_instruction(const std::string& mnemonic, const std::vector<Operand>& operands,
                        EncodedInst& out, std::string& error) {
  const auto* row = find_generated_row(mnemonic);
  if (!row) {
    error = std::string("unsupported mnemonic: ") + mnemonic;
    return false;
  }
  if (mnemonic == "nop" || mnemonic == "exit") {
    out = EncodedInst{row->template_lo, row->template_hi};
    return true;
  }
  if (mnemonic == "check") {
    if (operands.size() != 1 ||
        operands[0].kind != OperandKind::Raw ||
        operands[0].text != "k vm vs sm") {
      error = "check only supports the defined 'k vm vs sm' flags";
      return false;
    }
    out = EncodedInst{row->template_lo, row->template_hi};
    return true;
  }
  if (mnemonic == "sioexport") {
    return encode_sio_export(operands, out, error);
  }
  if (mnemonic == "siowrrot_imm") {
    return encode_siowrrot_imm(operands, out, error);
  }
  const auto* spec = semantic_spec(mnemonic);
  if (!spec) {
    error = std::string("no semantic rules for ") + mnemonic;
    return false;
  }
  return encode_generic(*row, *spec, operands, out, error);
}

bool decode_instruction(std::uint32_t lo, std::uint32_t hi, DecodedInst& out,
                        std::string& error) {
  const auto* row = find_generated_row_by_words(lo, hi);
  if (!row) {
    error = "encoding does not match any supported instruction row";
    return false;
  }
  const std::string mnemonic = row->mnemonic;
  if (mnemonic == "check") {
    if (lo == 0x0000000fu && hi == 0x40320000u) {
      out.mnemonic = "check";
      out.operands = {op_raw("k vm vs sm")};
      out.assembly = "check k vm vs sm";
      return true;
    }
    error = "check flags outside the defined tuple";
    return false;
  }
  if (mnemonic == "sioexport") {
    return decode_sio_export(lo, hi, out, error);
  }
  if (mnemonic == "siowrrot_imm") {
    return decode_siowrrot_imm(lo, hi, out, error);
  }
  if (mnemonic == "nop" || mnemonic == "exit") {
    out.mnemonic = mnemonic;
    out.assembly = mnemonic;
    return true;
  }
  const auto* spec = semantic_spec(mnemonic);
  if (!spec) {
    error = std::string("no semantic rules for ") + mnemonic;
    return false;
  }
  return decode_generic(*row, *spec, lo, hi, out, error);
}

}  // namespace lgpu::isa
