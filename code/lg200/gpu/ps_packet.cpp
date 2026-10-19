#include "ps_packet.hpp"

#include "generated/ps_control_packet.inc"

#include <cstring>

namespace lgpu::carrier {
namespace {

std::uint64_t extract_bits(const std::uint32_t* words, unsigned start,
                           unsigned bits) {
  std::uint64_t value = 0;
  unsigned consumed = 0;
  while (consumed < bits) {
    const unsigned bit = start + consumed;
    const unsigned word = bit / 32;
    const unsigned shift = bit % 32;
    const unsigned count =
        bits - consumed < 32 - shift ? bits - consumed : 32 - shift;
    const std::uint32_t mask =
        count == 32 ? 0xffffffffu : ((std::uint32_t{1} << count) - 1u);
    value |= static_cast<std::uint64_t>((words[word] >> shift) & mask) <<
             consumed;
    consumed += count;
  }
  return value;
}

bool shader_info_matches(const std::uint32_t* words,
                         std::uint32_t shader_type,
                         std::uint32_t vgpr_initial_count,
                         std::uint32_t code_size, std::uint64_t kernel_entry,
                         std::uint64_t rw_buffer, std::uint64_t const_buffer,
                         std::uint64_t constant,
                         std::uint64_t resource_sampler_descriptors,
                         std::uint64_t attributes) {
  return extract_bits(words, 32, 8) == shader_type &&
         extract_bits(words, 40, 8) == vgpr_initial_count &&
         extract_bits(words, 48, 16) == code_size &&
         extract_bits(words, 128, 64) == kernel_entry &&
         extract_bits(words, 256, 64) == rw_buffer &&
         extract_bits(words, 320, 64) == const_buffer &&
         extract_bits(words, 384, 64) == constant &&
         extract_bits(words, 448, 64) == resource_sampler_descriptors &&
         extract_bits(words, 512, 64) == attributes;
}

}  // namespace

namespace {
constexpr std::uint32_t kStageSlotBase[] = {
    0x5a0,  // Hs
    0x660,  // Gs
    0x6c0,  // Ss
    0x600,  // Ds
    0x540,  // Vs
    0x720,  // Ps
};

void write_bits(std::uint32_t* words, unsigned start, unsigned bits,
                std::uint64_t value) {
  unsigned written = 0;
  while (written < bits) {
    const unsigned bit = start + written;
    const unsigned word = bit / 32;
    const unsigned shift = bit % 32;
    const unsigned count =
        bits - written < 32 - shift ? bits - written : 32 - shift;
    const std::uint32_t mask =
        count == 32 ? 0xffffffffu : ((std::uint32_t{1} << count) - 1u);
    words[word] &= ~(mask << shift);
    words[word] |= static_cast<std::uint32_t>((value >> written) & mask)
                   << shift;
    written += count;
  }
}
}  // namespace

PsControlPacket::PsControlPacket() {
  std::copy(std::begin(generated::kPsControlPacket),
            std::end(generated::kPsControlPacket), packet_.begin());
}

bool PsControlPacket::patch(std::string& error) {
  // Unrelocated sanity: header, state terminator/end, color-buffer slot-0
  // geometry (base must be zero until relocated) and the DS/PS records.
  if (packet_[0] != 0x00000f04u || packet_[1] != 0x0000000au ||
      packet_[2] != 0x20000001u) {
    error = "GFX full-state header mismatch";
    return false;
  }
  if (packet_[515] != 3u || packet_[516] != 0x00000f05u) {
    error = "GFX state-end mismatch";
    return false;
  }
  const std::uint32_t* color = packet_.data() + 223;
  if (extract_bits(color, 96, 16) != 4 ||
      extract_bits(color, 112, 16) != 4 ||
      extract_bits(color, 128, 16) != 1 ||
      extract_bits(color, 144, 16) != 64 ||
      extract_bits(color, 196, 4) != 6) {
    error = "color buffer geometry mismatch";
    return false;
  }
  if (extract_bits(color, 0, 64) != 0) {
    error = "unrelocated color base must be zero";
    return false;
  }
  const std::uint32_t* ds = packet_.data() + 387;
  const std::uint32_t* ps = packet_.data() + 459;
  if (!shader_info_matches(ds, 3, 4, generated::kPsControlVsSize, 0, 0, 0,
                           0, 0, 0) ||
      !shader_info_matches(ps, 5, 15, generated::kPsControlFsSize, 0, 0, 0,
                           0, 0, 0)) {
    error = "unrelocated shader info mismatch";
    return false;
  }

  // Relocate the zero relocation slots against the fixed resource VAs.
  std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(packet_.data());
  for (const auto& relocation : generated::kPsControlRelocations) {
    const std::uint64_t address =
        resource_address(relocation.resource) + relocation.addend;
    std::uint64_t zero = 0;
    std::memcpy(&zero, bytes + relocation.offset, sizeof(zero));
    if (zero != 0) {
      error = "relocation slot is not all-zero";
      return false;
    }
    std::memcpy(bytes + relocation.offset, &address, sizeof(address));
  }

  // Relocated validation: color base -> result BO, DS/PS records -> the
  // fixed VS/FS program and arg-BO roots.
  const std::uint64_t result = resource_address(1);
  const std::uint64_t vs = resource_address(2);
  const std::uint64_t fs = resource_address(3);
  const std::uint64_t arg = resource_address(0);
  if (extract_bits(color, 0, 64) != result) {
    error = "color base relocation mismatch";
    return false;
  }
  if (fs_code_size_ % 16 != 0) {
    error = "PS shader code_size is not 16-byte aligned";
    return false;
  }
  if (!shader_info_matches(ds, 3, 4, generated::kPsControlVsSize, vs, arg,
                           arg + 0x100, arg + 0x100, arg + 0x200,
                           arg + 0x400) ||
      !shader_info_matches(ps, 5, 15, fs_code_size_, fs, arg, arg + 0x100,
                           arg + 0x100, arg + 0x200, 0)) {
    error = "shader info relocation mismatch";
    return false;
  }
  return true;
}

void PsControlPacket::set_fs_code_size(std::uint32_t code_size) {
  fs_code_size_ = code_size;
  const unsigned word = 459 + 48 / 32;
  const unsigned shift = 48 % 32;
  packet_[word] &= ~(0xffffu << shift);
  packet_[word] |= (code_size & 0xffffu) << shift;
}

void PsControlPacket::set_ps_roots_full(std::uint64_t const_buffer,
                                        std::uint64_t constant,
                                        std::uint64_t rsd) {
  std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(packet_.data());
  std::memcpy(bytes + 0x754, &const_buffer, sizeof(const_buffer));
  std::memcpy(bytes + 0x75c, &constant, sizeof(constant));
  std::memcpy(bytes + 0x764, &rsd, sizeof(rsd));
}

std::uint64_t PsControlPacket::resource_address(std::uint32_t resource) {
  if (resource >= kResourceCount) {
    return 0;
  }
  return generated::kPsControlResourceAddresses[resource];
}

void PsControlPacket::write_bits_at(std::uint32_t byte_off, unsigned start,
                                     unsigned bits, std::uint64_t value) {
  std::uint32_t* words = packet_.data() + byte_off / 4;
  unsigned written = 0;
  while (written < bits) {
    const unsigned bit = start + written;
    const unsigned word = bit / 32;
    const unsigned shift = bit % 32;
    const unsigned count =
        bits - written < 32 - shift ? bits - written : 32 - shift;
    const std::uint32_t mask =
        count == 32 ? 0xffffffffu : ((std::uint32_t{1} << count) - 1u);
    words[word] &= ~(mask << shift);
    words[word] |= static_cast<std::uint32_t>((value >> written) & mask)
                   << shift;
    written += count;
  }
}

void PsControlPacket::set_stage_config(std::uint32_t config) {
  // The stage-enable/fusion word lives at packet byte 0x4cc; the template
  // already carries the 0x24 baseline (ordinary DS-as-VS).
  packet_[0x4cc / 4] = config;
}

std::uint32_t PsControlPacket::stage_record_base(ShaderStage stage) {
  const std::uint32_t index = static_cast<std::uint32_t>(stage);
  if (index > static_cast<std::uint32_t>(ShaderStage::Ps)) {
    return 0;
  }
  return kStageSlotBase[index] + 0x0c;
}

std::uint64_t PsControlPacket::stage_program_address(ShaderStage stage) {
  // HS/GS/SS executable BOs sit above the MRT color range
  // (0x100018000..0x100024000); the DS stage-program BO (doc 63 real
  // domain-shader record) sits above the HS BO, and the VS-slot
  // observability BO above that.
  switch (stage) {
    case ShaderStage::Gs:
      return 0x100024000ull;
    case ShaderStage::Ss:
      return 0x100028000ull;
    case ShaderStage::Hs:
      return 0x10002c000ull;
    case ShaderStage::Ds:
      return 0x100030000ull;
    case ShaderStage::Vs:
      return 0x100034000ull;
    default:
      return 0;
  }
}

void PsControlPacket::set_stage_record(ShaderStage stage,
                                       std::uint32_t shader_type,
                                       std::uint32_t code_size,
                                       std::uint8_t vgpr_count,
                                       std::uint64_t kernel_entry,
                                       std::uint64_t const_buffer,
                                       std::uint64_t constant,
                                       std::uint64_t rsd,
                                       std::uint64_t rw_buffer,
                                       std::uint64_t attributes,
                                       std::uint32_t word0,
                                       std::uint32_t tail) {
  const std::uint32_t base = stage_record_base(stage);
  if (base == 0) {
    return;
  }
  std::uint32_t* words = packet_.data() + base / 4;
  words[0] = word0 ? word0
                   : (stage == ShaderStage::Gs ? 0x00004010u : 0x00000810u);
  write_bits(words, 32, 8, shader_type);
  write_bits(words, 40, 8, vgpr_count);
  write_bits(words, 48, 16, code_size);
  write_bits(words, 128, 64, kernel_entry);
  write_bits(words, 256, 64, rw_buffer);
  write_bits(words, 320, 64, const_buffer);
  write_bits(words, 384, 64, constant);
  write_bits(words, 448, 64, rsd);
  write_bits(words, 512, 64, attributes);
  if (tail) {
    words[0x5c / 4] = tail;
  }
}
}  // namespace lgpu::carrier
