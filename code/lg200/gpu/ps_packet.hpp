#pragma once

// The one packet builder used by the drm layer: the reviewed PS-control
// template + relocation list from generated/ps_control_packet.inc, loaded,
// patched (patch()) and root-overridden (set_ps_roots_full) here.  Only the
// fragment-carrier subset of the conformance packet API is kept.

#include <array>
#include <cstdint>
#include <string>

#include "generated/ps_control_packet.inc"

namespace lgpu::carrier {

class PsControlPacket {
 public:
  static constexpr std::uint32_t kPacketDwords = 528;
  static constexpr std::uint32_t kPageSize = 16384;
  static constexpr std::uint32_t kImageSize = 1024;
  static constexpr std::uint32_t kResourceCount = 6;

  enum class ShaderStage { Hs, Gs, Ss, Ds, Vs, Ps };

  PsControlPacket();

  // Relocates the template (validate -> write -> re-validate) against the
  // six fixed resource addresses.  The PS shader-info record lives at
  // packet bytes 0x72c: const_buffer +0x28 (0x754), constant +0x30 (0x75c),
  // rsd +0x38 (0x764); all other records keep their relocated template
  // roots.
  bool patch(std::string& error);
  void set_fs_code_size(std::uint32_t code_size);
  void set_ps_roots_full(std::uint64_t const_buffer, std::uint64_t constant,
                         std::uint64_t rsd);

  // Stage-enable/fusion word (packet byte 0x4cc): 0x24 ordinary DS-as-VS
  // draw, 0x2c/0x34 with GS/SS enable (closed captures; gs-contract and
  // query-streamout campaigns), plus GS output-topology bits 0x1000/0x2000/
  // 0x3000.  kShaderConfigGsEnable = 1<<3.
  void set_stage_config(std::uint32_t config);

  // Writes one stage shader-info record (HS/GS/SS/DS/VS slots).  The GS
  // record word0 default is 0x4010 (LDS size 4, closed gs-passthrough);
  // other slots mirror the DS template word0 0x810.
  void set_stage_record(ShaderStage stage, std::uint32_t shader_type,
                        std::uint32_t code_size, std::uint8_t vgpr_count,
                        std::uint64_t kernel_entry, std::uint64_t const_buffer,
                        std::uint64_t constant, std::uint64_t rsd,
                        std::uint64_t rw_buffer = 0,
                        std::uint64_t attributes = 0,
                        std::uint32_t word0 = 0, std::uint32_t tail = 0);

  // Bit-field writer over the raw packet dwords (bit start, width).
  void write_bits_at(std::uint32_t byte_off, unsigned start, unsigned bits,
                     std::uint64_t value);

  // Executable BO addresses for the extra stage programs (above the MRT
  // color range); the DS stage-program BO sits above the HS BO.
  static std::uint64_t stage_program_address(ShaderStage stage);

  // The six fixed carrier resource VAs (from the generated relocation list).
  static std::uint64_t resource_address(std::uint32_t resource);

  const std::uint32_t* data() const { return packet_.data(); }

 private:
  static std::uint32_t stage_record_base(ShaderStage stage);

  std::array<std::uint32_t, kPacketDwords> packet_;
  std::uint32_t fs_code_size_ = generated::kPsControlFsSize;
};

}  // namespace lgpu::carrier
