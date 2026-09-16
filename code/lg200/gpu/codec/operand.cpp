#include "operand.hpp"

#include <cstdio>

namespace lgpu::isa {

std::string format_reg(const Reg& reg) {
  char buffer[32];
  switch (reg.cls) {
    case RegClass::Vector:
      if (reg.width > 1) {
        std::snprintf(buffer, sizeof(buffer), "v%u-v%u", reg.index,
                      static_cast<unsigned>(reg.index) + reg.width - 1);
      } else {
        std::snprintf(buffer, sizeof(buffer), "v%u", reg.index);
      }
      break;
    case RegClass::Scalar:
      if (reg.width > 1) {
        std::snprintf(buffer, sizeof(buffer), "s%u-s%u", reg.index,
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
      if (operand.text == "sio") {
        char buffer[16];
        std::snprintf(buffer, sizeof(buffer), "SIO%u", operand.value);
        return buffer;
      }
      if (operand.text == "channum" || operand.text == "chan") {
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "%s(%u)",
                      operand.text.c_str(), operand.value);
        return buffer;
      }
      if (operand.text == "dmask") {
        std::string text = "dmask:";
        for (int bit = 3; bit >= 0; --bit) {
          text += ((operand.value >> bit) & 1u) ? '1' : '0';
        }
        return text;
      }
      if (operand.value == 1) {
        return operand.text;
      }
      char buffer[64];
      std::snprintf(buffer, sizeof(buffer), "%s(%u)", operand.text.c_str(), operand.value);
      return buffer;
    }
    case OperandKind::Raw:
      return operand.text;
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
