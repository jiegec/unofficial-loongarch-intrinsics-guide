#include "operand.hpp"

#include <cstdio>

namespace lgpu::isa {

std::string format_reg(const Reg& reg) {
  char buffer[32];
  switch (reg.cls) {
    case RegClass::Vector:
      // Bracketed range, the form both LLVM backends use: `v[0:3]`.  Both the
      // vendor's own lggpu printer and AMDGPU write tuples this way, so `v0-v3`
      // was this repo's invention and forced a translation on every reader.
      if (reg.width > 1) {
        std::snprintf(buffer, sizeof(buffer), "v[%u:%u]", reg.index,
                      static_cast<unsigned>(reg.index) + reg.width - 1);
      } else {
        std::snprintf(buffer, sizeof(buffer), "v%u", reg.index);
      }
      break;
    case RegClass::Scalar:
      if (reg.width > 1) {
        std::snprintf(buffer, sizeof(buffer), "s[%u:%u]", reg.index,
                      static_cast<unsigned>(reg.index) + reg.width - 1);
      } else {
        std::snprintf(buffer, sizeof(buffer), "s%u", reg.index);
      }
      break;
    case RegClass::Predicate:
      std::snprintf(buffer, sizeof(buffer), "p%u", reg.index);
      break;
    case RegClass::Vcc:
      std::snprintf(buffer, sizeof(buffer), "vcc(s%u)", reg.index);
      break;
    case RegClass::SpecialZero:
      return "zero";
    case RegClass::Scc:
      return "SCC";
  }
  return buffer;
}

std::string format_operand(const Operand& operand) {
  switch (operand.kind) {
    case OperandKind::Reg:
      return format_reg(operand.reg);
    case OperandKind::Immediate: {
      char buffer[16];
      std::snprintf(buffer, sizeof(buffer), "0x%08x", operand.value);
      return buffer;
    }
    case OperandKind::ShortImmediate: {
      char buffer[8];
      std::snprintf(buffer, sizeof(buffer), "0x%03x", operand.value);
      return buffer;
    }
    case OperandKind::Immediate16: {
      char buffer[8];
      std::snprintf(buffer, sizeof(buffer), "0x%04x", operand.value);
      return buffer;
    }
    case OperandKind::Immediate8: {
      char buffer[8];
      std::snprintf(buffer, sizeof(buffer), "0x%02x", operand.value);
      return buffer;
    }
    case OperandKind::Immediate20: {
      char buffer[8];
      std::snprintf(buffer, sizeof(buffer), "0x%05x", operand.value);
      return buffer;
    }
    case OperandKind::InlineInt: {
      char buffer[16];
      std::snprintf(buffer, sizeof(buffer), "i(%d)", static_cast<int32_t>(operand.value));
      return buffer;
    }
    case OperandKind::InlineFloat: {
      static constexpr struct {
        std::uint32_t bits;
        const char* text;
      } kFloats[] = {
          {0x3f800000u, "1.0"}, {0x40000000u, "2.0"},
          {0xbf800000u, "-1.0"}, {0xc0000000u, "-2.0"},
      };
      for (const auto& entry : kFloats) {
        if (entry.bits == operand.value) {
          return std::string("f(") + entry.text + ")";
        }
      }
      char buffer[16];
      std::snprintf(buffer, sizeof(buffer), "f(0x%08x)", operand.value);
      return buffer;
    }
    case OperandKind::Modifier: {
      // Only the one-bit enable flags are omitted when clear.  A *valued*
      // selector must print at zero: `sio:0` and `channum:0` are meaningful
      // (SIO0 is a real stream, channel 0 a real channel), and dropping them
      // silently loses information a reader needs to reconstruct the encoding.
      // These are the modifier *names* the rules and the SIO codecs use, not
      // the M.* class names - `un`, not `M.un1`.
      const bool is_flag =
          operand.text == "bl1" || operand.text == "bl2" ||
          operand.text == "clmp1" || operand.text == "idxen" ||
          operand.text == "na" || operand.text == "offen" ||
          operand.text == "rsz" || operand.text == "un" ||
          operand.text == "vm";
      if (is_flag) {
        return operand.value != 0 ? operand.text : std::string();
      }
      // A mask reads as hex, a count or an index reads as decimal - the split
      // AMDGPU uses (`dmask:0xf` against `offset:4`).  Both are unambiguous;
      // the difference is only which one a reader parses faster.
      char buffer[64];
      if (operand.text == "dmask") {
        std::snprintf(buffer, sizeof(buffer), "dmask:0x%x", operand.value);
      } else {
        std::snprintf(buffer, sizeof(buffer), "%s:%u", operand.text.c_str(),
                      operand.value);
      }
      return buffer;
    }
    case OperandKind::Raw:
      return operand.text;
    case OperandKind::RawSelector: {
      // Probe-only operand: print it as an explicit byte so a probe listing
      // can never be mistaken for a named selector.
      char buffer[16];
      std::snprintf(buffer, sizeof(buffer), "raw(0x%02x)", operand.value);
      return buffer;
    }
  }
  return "<invalid>";
}

std::string McInst::format() const {
  std::string result = mnemonic;
  if (!prefix.empty()) {
    result += " ";
    result += prefix;
  }
  for (std::size_t i = 0; i < operands.size(); ++i) {
    result += (i == 0 ? " " : ", ");
    result += format_operand(operands[i]);
  }
  return result;
}

}  // namespace lgpu::isa
