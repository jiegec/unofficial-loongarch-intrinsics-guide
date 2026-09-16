#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace lgpu::isa {

enum class RegClass {
  Vector,
  Scalar,
  Predicate,
  Vcc,
  SpecialZero,
  Scc,
};

struct Reg {
  RegClass cls = RegClass::Vector;
  std::uint16_t index = 0;
  std::uint8_t width = 1;
};

inline Reg vgpr(std::uint16_t index, std::uint8_t width = 1) {
  return {RegClass::Vector, index, width};
}
inline Reg sgpr(std::uint16_t index, std::uint8_t width = 1) {
  return {RegClass::Scalar, index, width};
}
inline Reg predicate(std::uint16_t index) { return {RegClass::Predicate, index, 1}; }
inline Reg vcc(std::uint16_t scalar_index) { return {RegClass::Vcc, scalar_index, 2}; }
inline Reg zero_reg() { return {RegClass::SpecialZero, 0, 1}; }
inline Reg scc_reg() { return {RegClass::Scc, 128, 1}; }

enum class OperandKind {
  Reg,
  Immediate,
  ShortImmediate,
  Immediate8,
  Immediate16,
  Immediate20,
  InlineInt,
  InlineFloat,
  Modifier,
  Raw,
};

struct Operand {
  OperandKind kind = OperandKind::Reg;
  Reg reg;
  std::uint32_t value = 0;
  std::string text;
  std::string field;
};

inline Operand op_reg(const Reg& reg) {
  Operand out;
  out.kind = OperandKind::Reg;
  out.reg = reg;
  return out;
}

inline Operand op_imm(std::uint32_t value) {
  Operand out;
  out.kind = OperandKind::Immediate;
  out.value = value;
  return out;
}

inline Operand op_short_imm(std::uint32_t value) {
  Operand out;
  out.kind = OperandKind::ShortImmediate;
  out.value = value;
  return out;
}

inline Operand op_imm8(std::uint32_t value) {
  Operand out;
  out.kind = OperandKind::Immediate8;
  out.value = value;
  return out;
}

inline Operand op_imm16(std::uint32_t value) {
  Operand out;
  out.kind = OperandKind::Immediate16;
  out.value = value;
  return out;
}

inline Operand op_imm20(std::uint32_t value) {
  Operand out;
  out.kind = OperandKind::Immediate20;
  out.value = value;
  return out;
}

inline Operand op_imm8_unused(std::uint32_t value) {
  Operand out;
  out.kind = OperandKind::Immediate8;
  out.value = value;
  return out;
}

// Inline integer constant: value holds the signed integer (n in 1..41 or -1
// in canonical forms).
inline Operand op_inline_int(std::uint32_t value) {
  Operand out;
  out.kind = OperandKind::InlineInt;
  out.value = value;
  return out;
}

// Inline float constant: value holds the f32 bits (1.0/2.0/-1.0/-2.0 per
// canonical forms).
inline Operand op_inline_float(std::uint32_t bits) {
  Operand out;
  out.kind = OperandKind::InlineFloat;
  out.value = bits;
  return out;
}

inline Operand op_modifier(const std::string& name, std::uint32_t value) {
  Operand out;
  out.kind = OperandKind::Modifier;
  out.value = value;
  out.text = name;
  return out;
}

inline Operand op_raw(std::string text) {
  Operand out;
  out.kind = OperandKind::Raw;
  out.text = std::move(text);
  return out;
}

std::string format_reg(const Reg& reg);
std::string format_operand(const Operand& operand);

struct McInst {
  std::string mnemonic;
  std::string prefix = "";
  std::vector<Operand> operands;

  std::string format() const;
};

struct EncodedInst {
  std::uint32_t lo = 0;
  std::uint32_t hi = 0;

  bool operator==(const EncodedInst&) const = default;
};

}  // namespace lgpu::isa
