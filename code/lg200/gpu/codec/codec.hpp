#pragma once

#include "operand.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace lgpu::isa {

// Exact-variant view (canonical assembly + words) used by the carrier's
// register-class resolution and the --matrix dump.
struct ExactVariant {
  std::string variant_id;
  std::string mnemonic;
  std::string assembly;
  EncodedInst words;
};

struct DecodedInst {
  std::string mnemonic;
  std::vector<Operand> operands;
  std::string assembly;
};

std::vector<ExactVariant> codec_variants();

bool encode_instruction(const std::string& mnemonic, const std::vector<Operand>& operands,
                        EncodedInst& out, std::string& error);
bool decode_instruction(std::uint32_t lo, std::uint32_t hi, DecodedInst& out,
                        std::string& error);

}  // namespace lgpu::isa
