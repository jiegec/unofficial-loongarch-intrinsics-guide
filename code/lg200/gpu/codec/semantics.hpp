#pragma once

#include <cstdint>
#include <string>

namespace lgpu::isa {

enum class FieldSem {
  Predicate2,
  VectorDest8,
  VectorSource8,
  GeneralSource11,
  GeneralSource8,
  Scalar8,
  // 8-bit scalar compare destination: 0x00-0x7F scalar register, 0x80 SCC,
  // 102 is zero. RD8 == 0x80 prints as "SCC" for s_cmp_*.
  ScalarCmp8,
  ScalarQuad8,
  OffsetSource8,
  Vcc8,
  Immediate32,
  Immediate40,  // control branch X0_40 (LO[31:0] + HI[7:0]), prints 0x%010llx
  Immediate16,  // SIO 16-bit slot (IA16/IK16/IJ16/X2_16), prints 0x%04x
  Immediate12,
  Immediate20,  // smem-load UO20 slot, prints 0x%05x
  Immediate23,  // jump-stack X2_23 slot, prints 0x%06x
  Immediate8,
  // 8-bit general source accepting the "SCC" selector encoding 0x80.
  SccSource8,
  // SIO slot semantics used by immediate rotate/write forms.
  SioId,
  SioChannum,
  SioChan,
  Modifier,
};

struct FieldRule {
  const char* field;
  FieldSem sem;
  std::uint8_t tuple_width = 1;
  const char* modifier_name = nullptr;
  bool is_output = false;
  bool omit_zero = false;
};

struct SemanticSpec {
  const char* mnemonic;
  const char* asm_prefix;  // printed between mnemonic and first operand
  const FieldRule* rules;
  unsigned rule_count;
};

// Resolves the effective tuple width for a rule of a mnemonic, applying the
// per-mnemonic overrides derived from canonical forms.
std::uint8_t semantic_tuple(const char* mnemonic, const char* field,
                            std::uint8_t fallback);

const SemanticSpec* semantic_spec(const std::string& mnemonic);

}  // namespace lgpu::isa
