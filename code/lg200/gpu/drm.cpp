// Minimal PS-carrier DRM submission path.
//
// This layer is deliberately opaque about shader semantics: the PS program
// arrives as assembled dwords and is written verbatim into the fragment
// shader BO. It does NOT import the ISA codec, the shader builder, or any
// per-case knowledge. Per-case isolation (one process per case) is the
// CALLER's responsibility; each submit_ps()/submit_probe() call performs
// exactly one submission.
//
// Both public entry points share one implementation: the 528-dword GFX
// full-state IB is always built from the generated PsControlPacket template
// (single source), relocated and validated by patch(), and the six carrier
// BOs / submit / fence / readback / health flow lives in one place.

#include "carrier_map.hpp"
#include "drm.hpp"
#include "mcs/loongvk_mcs.h"
#include "mcs/mcs_kd.h"
#include "ps_packet.hpp"

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

#ifdef LGPU_HAVE_LIBDRM
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#include <loonggpu.h>
#include <loonggpu_drm.h>
#endif

namespace lgpu::drm {

#ifdef LGPU_HAVE_LIBDRM
namespace {

// Page/image/ASIC constants come from the single generated packet source.
constexpr std::uint32_t kPageSize = carrier::PsControlPacket::kPageSize;
constexpr std::uint32_t kImageSize = carrier::PsControlPacket::kImageSize;
constexpr std::uint32_t kResourceCount =
    carrier::PsControlPacket::kResourceCount;
constexpr std::uint32_t kPacketDwords =
    carrier::PsControlPacket::kPacketDwords;
constexpr std::uint32_t kAsicId = carrier::generated::kPsControlAsicId;

// Compute-carrier GPU VA.  The PS carrier owns 0x100000000..0x100018000 (six
// 16 KiB resources starting at kArgVa), and these BOs are mapped at fixed
// addresses rather than through libdrm's VA manager, so the compute window is
// placed clear of that block by hand.  One BO carries the 64-byte user-queue
// index block followed by the ring, so the ring's own VA is this plus 64.
constexpr std::uint64_t kCsRingBaseVa = 0x100020000ull;

// The rest of the compute carrier's GPU VAs, continuing upward from the ring.
// Each is a separate BO because their lifetimes differ: the ring and the
// kernel descriptor outlive a single dispatch, the kernarg and the completion
// signal do not.  Nothing may land between these and the PS block below
// 0x100018000, and fixed-address mapping means a collision would not be
// detected until the VA map failed.
constexpr std::uint64_t kCsTextVa = 0x100080000ull;
constexpr std::uint64_t kCsKdVa = 0x100090000ull;
constexpr std::uint64_t kCsKernargVa = 0x1000a0000ull;
constexpr std::uint64_t kCsOutputVa = 0x1000b0000ull;
constexpr std::uint64_t kCsSignalVa = 0x1000c0000ull;

// Register counts declared in rsrc1 and the .kd.  LOONGVK_KD_RSRC1 already
// carries kernel_sgpr_cnt = 11, the SGPR where the compiled compute shader
// reads group_id_x (LCO_ABI_CS_GROUP_ID_SGPR); these two numbers are only the
// allocation sizes, and granulated(8, 8) == 0x42 matches the vendor's word for
// the add.cl dispatch the MCS vector came from.
constexpr std::uint32_t kCsNumSgprs = 8;
constexpr std::uint32_t kCsNumVgprs = 8;

// The CPIPE ring.  LoongVK uses 64 KiB; the kernel's own bounds are 4 KiB to
// 1 MiB, and a stream is at most LOONGVK_MCS_MAX_LEN, so this is generous
// rather than tight - the ring only ever holds one stream at a time here.
constexpr std::uint32_t kCsRingSize = 64u * 1024u;

// The completion signal allocation.  Only the first eight bytes are the counter
// the carrier waits on; the rest exists so that a firmware-written timestamp
// block, if one is written at all, falls inside a mapping we can read.
constexpr std::uint32_t kCsSignalBytes = 4096u;

// The private-buffer descriptor the vendor hardcodes for a dispatch with no
// scratch region.  An input, not something the builder derives: its last dword
// carries the .kd flag bits in its low half.
constexpr std::uint32_t kCsNoScratchDescriptor[4] = { 0u, 0x80000000u, 0u,
                                                      0x00ca4facu };

// Fills a loongvk_mcs_dispatch for the carrier's fixed layout.  Single place
// where the VA constants and the descriptor defaults meet, so the host-side
// plan and the real submit cannot disagree about them.
void make_cs_dispatch(loongvk_mcs_dispatch& d, std::uint32_t group_x,
                      std::uint32_t group_y, std::uint32_t group_z,
                      std::uint32_t grid_x, std::uint32_t grid_y,
                      std::uint32_t grid_z) {
  d = loongvk_mcs_dispatch{};
  d.kd_flags = LOONGVK_KD_FLAGS;
  d.kernel_object = kCsKdVa;
  d.entry = kCsTextVa;
  d.kernarg_address = kCsKernargVa;
  d.completion_signal = kCsSignalVa;
  d.num_sgprs = kCsNumSgprs;
  d.num_vgprs = kCsNumVgprs;
  d.shared_segment_size = 0;
  d.group_size[0] = static_cast<std::uint16_t>(group_x);
  d.group_size[1] = static_cast<std::uint16_t>(group_y);
  d.group_size[2] = static_cast<std::uint16_t>(group_z);
  d.grid_size[0] = grid_x;
  d.grid_size[1] = grid_y;
  d.grid_size[2] = grid_z;
  d.group_aperture_hi = 0x00010000u;
  d.private_aperture_hi = 0x00020000u;
  for (int i = 0; i < 4; ++i) {
    d.scratch_resource_descriptor[i] = kCsNoScratchDescriptor[i];
  }
  d.float_mode = LOONGVK_KD_FLOAT_MODE_DEFAULT;
}

// Fixed GPU VAs of the six carrier resources: single source is the generated
// packet relocation list (carrier::PsControlPacket::resource_address()).
//   0 argument BO (rw: PS store target + preload)
//   1 result/color BO (4x4 RGBA8 render target)
//   2 VS program BO (fixed DS-as-VS vertex shader)
//   3 FS program BO (the submitted PS program)
//   4 packet BO (the GFX full-state indirect buffer)
//   5 guard BO (0xa5 canary, detects out-of-bounds writes)

// Builds the relocated PS-control packet for one FS program size.  patch()
// relocates and validates against the DEFAULT roots; the PS const/constant/
// rsd roots are applied afterwards without re-validation, exactly like the
// proven conformance carrier.  submit_ps passes the template roots (arg +
// 0x100/0x100/0x200), submit_probe the probe roots (arg + 0x600/0x300/0x200),
// so one builder serves both paths.
bool build_packet(std::array<std::uint32_t, kPacketDwords>& packet,
                  std::uint32_t fs_code_size, std::uint64_t const_buffer,
                  std::uint64_t constant, std::uint64_t rsd,
                  std::string& error) {
  carrier::PsControlPacket p;
  if (!p.patch(error)) return false;
  p.set_fs_code_size(fs_code_size);
  p.set_ps_roots_full(const_buffer, constant, rsd);
  // Fragment input config (packet 0x530, 2 dwords): smooth coefficient
  // window 0x4002 with 8 coefficient planes (loongvk defaults).  This makes
  // the interpolator (load_sm_interp / v_interp_*) addressable on the PS
  // carrier; ordinary PS programs do not consume it.
  // LGPU_INPUT_ADDR overrides the coefficient window offset (probe switch).
  const std::uint32_t input_addr =
      std::getenv("LGPU_INPUT_ADDR") ? static_cast<std::uint32_t>(
                                           std::strtoul(std::getenv("LGPU_INPUT_ADDR"), nullptr, 0))
                                     : 0x4002u;
  p.write_bits_at(0x530, 0, 16, input_addr);  // input_address
  p.write_bits_at(0x530, 16, 16,
                  std::getenv("LGPU_NO_INPUT_ENABLE") ? 0u : 0x4002u);
  p.write_bits_at(0x530, 36, 6, 8u);         // coefficient_count
  std::memcpy(packet.data(), p.data(), sizeof(packet));
  return true;
}

std::vector<std::uint8_t> words_to_bytes(
    const std::vector<std::uint32_t>& words) {
  std::vector<std::uint8_t> bytes(words.size() * 4);
  for (std::size_t i = 0; i < words.size(); ++i) {
    const std::uint32_t w = words[i];
    bytes[4 * i + 0] = static_cast<std::uint8_t>(w);
    bytes[4 * i + 1] = static_cast<std::uint8_t>(w >> 8);
    bytes[4 * i + 2] = static_cast<std::uint8_t>(w >> 16);
    bytes[4 * i + 3] = static_cast<std::uint8_t>(w >> 24);
  }
  return bytes;
}

struct Bo {
  loonggpu_bo_handle handle = nullptr;
  loonggpu_va_handle va = nullptr;
  std::uint64_t address = 0;
  std::uint64_t size = kPageSize;
  bool mapped = false;

  Bo() = default;
  Bo(const Bo&) = delete;
  Bo& operator=(const Bo&) = delete;
  Bo(Bo&& other) noexcept
      : handle(other.handle), va(other.va), address(other.address),
        size(other.size), mapped(other.mapped) {
    other.handle = nullptr;
    other.va = nullptr;
    other.mapped = false;
  }
  Bo& operator=(Bo&& other) noexcept {
    if (this != &other) {
      handle = other.handle;
      va = other.va;
      address = other.address;
      size = other.size;
      mapped = other.mapped;
      other.handle = nullptr;
      other.va = nullptr;
      other.mapped = false;
    }
    return *this;
  }
  ~Bo() {
    if (mapped) {
      loonggpu_bo_va_op(handle, 0, size, address, 0, LOONGGPU_VA_OP_UNMAP);
    }
    if (va) {
      loonggpu_va_range_free(va);
    }
    if (handle) {
      loonggpu_bo_free(handle);
    }
  }
};

bool create_resource_at(loonggpu_device_handle device, Bo& bo,
                        std::uint64_t wanted, std::uint64_t vm_flags,
                        const void* payload, std::size_t payload_size,
                        std::uint8_t fill, std::string& error) {
  loonggpu_bo_alloc_request request{};
  request.alloc_size = kPageSize;
  request.phys_alignment = kPageSize;
  request.preferred_heap = LOONGGPU_GEM_DOMAIN_GTT;
  request.flags = LOONGGPU_GEM_CREATE_CPU_ACCESS_REQUIRED;
  if (payload_size > kPageSize) {
    error = "payload exceeds one GPU page";
    return false;
  }
  if (loonggpu_bo_alloc(device, &request, &bo.handle) || !bo.handle) {
    error = "loonggpu_bo_alloc failed";
    return false;
  }
  void* map = nullptr;
  if (loonggpu_bo_cpu_map(bo.handle, &map) || !map) {
    error = "loonggpu_bo_cpu_map failed";
    return false;
  }
  std::memset(map, fill, kPageSize);
  if (payload_size && payload) {
    std::memcpy(map, payload, payload_size);
  }
  loonggpu_bo_cpu_unmap(bo.handle);

  // Map directly at the fixed GPU address.  libdrm's userspace VA manager is
  // deliberately not used: allocating six contiguous fixed ranges through
  // loonggpu_va_range_alloc() corrupts its hole list on teardown (crash in
  // loonggpu_vamgr_free() at the guard range).  GEM_VA maps a BO at any
  // user-chosen address, so fixed addresses are applied directly.
  bo.address = wanted;
  bo.va = nullptr;  // no va-manager range owned by this BO
  if (loonggpu_bo_va_op(bo.handle, 0, bo.size, bo.address, vm_flags,
                        LOONGGPU_VA_OP_MAP)) {
    error = "loonggpu_bo_va_op MAP failed";
    return false;
  }
  bo.mapped = true;
  return true;
}

// Allocate a BO of an arbitrary size and map it at a fixed GPU VA.  Page
// granularity is a hard requirement of loonggpu_bo_va_op, so the size is
// rounded up; a caller that needs a byte count should keep its own.
bool alloc_bo_at(loonggpu_device_handle device, Bo& bo, std::uint64_t va,
                 std::uint64_t bytes, std::uint8_t fill, std::string& error) {
  const std::uint64_t size = ((bytes + kPageSize - 1) / kPageSize) * kPageSize;
  loonggpu_bo_alloc_request request{};
  request.alloc_size = size;
  request.phys_alignment = kPageSize;
  request.preferred_heap = LOONGGPU_GEM_DOMAIN_GTT;
  request.flags = LOONGGPU_GEM_CREATE_CPU_ACCESS_REQUIRED;
  if (loonggpu_bo_alloc(device, &request, &bo.handle) || !bo.handle) {
    error = "BO allocation failed";
    return false;
  }
  void* map = nullptr;
  if (loonggpu_bo_cpu_map(bo.handle, &map) || !map) {
    error = "BO cpu map failed";
    return false;
  }
  std::memset(map, fill, size);
  loonggpu_bo_cpu_unmap(bo.handle);

  bo.size = size;
  bo.address = va;
  bo.va = nullptr;  // fixed-address mapping; no va-manager range is owned
  if (loonggpu_bo_va_op(bo.handle, 0, size, va, 0, LOONGGPU_VA_OP_MAP)) {
    error = "BO VA map failed";
    return false;
  }
  bo.mapped = true;
  return true;
}

// Write bytes into a BO's CPU mapping without disturbing the rest of it.
bool poke_bo(loonggpu_bo_handle bo, std::uint64_t offset,
             const void* bytes, std::size_t size, std::string& error) {
  void* map = nullptr;
  if (loonggpu_bo_cpu_map(bo, &map) || !map) {
    error = "BO map for write failed";
    return false;
  }
  std::memcpy(static_cast<std::uint8_t*>(map) + offset, bytes, size);
  loonggpu_bo_cpu_unmap(bo);
  return true;
}

bool check_bo_pattern(loonggpu_bo_handle bo, std::size_t offset,
                      std::size_t size, std::uint8_t value,
                      std::string& error) {
  void* map = nullptr;
  if (loonggpu_bo_cpu_map(bo, &map) || !map) {
    error = "map failed for pattern check";
    return false;
  }
  const auto* bytes = static_cast<const std::uint8_t*>(map);
  bool ok = true;
  for (std::size_t i = 0; i < size; ++i) {
    if (bytes[offset + i] != value) {
      ok = false;
      break;
    }
  }
  loonggpu_bo_cpu_unmap(bo);
  if (!ok) {
    error = "BO pattern mismatch";
    return false;
  }
  return true;
}

const char* drm_node() {
  const char* node = std::getenv("LGPU_DRM_NODE");
  return node != nullptr ? node : "/dev/dri/renderD128";
}

// Opens the node, initializes the device, checks ASIC identity, and creates
// a submit context.
bool open_session(int& fd, loonggpu_device_handle& device,
                  loonggpu_context_handle& ctx, std::string& error) {
  fd = open(drm_node(), O_RDWR | O_CLOEXEC);
  if (fd < 0) {
    error = std::string("cannot open ") + drm_node();
    return false;
  }
  std::uint32_t drm_major = 0, drm_minor = 0;
  if (loonggpu_device_initialize(fd, &drm_major, &drm_minor, &device) ||
      !device) {
    error = "loonggpu_device_initialize failed";
    close(fd);
    fd = -1;
    return false;
  }
  drm_loonggpu_info_device gpu{};
  if (loonggpu_query_info(device, LOONGGPU_INFO_DEV_INFO, sizeof(gpu), &gpu) ||
      gpu.device_id != kAsicId || drm_major != 1 || drm_minor != 0) {
    error = "unexpected DRM/ASIC identity";
    loonggpu_device_deinitialize(device);
    device = nullptr;
    close(fd);
    fd = -1;
    return false;
  }
  if (loonggpu_cs_ctx_create(device, &ctx) || !ctx) {
    error = "loonggpu_cs_ctx_create failed";
    loonggpu_device_deinitialize(device);
    device = nullptr;
    close(fd);
    fd = -1;
    return false;
  }
  return true;
}

void close_session(int fd, loonggpu_device_handle device,
                   loonggpu_context_handle ctx) {
  if (ctx) {
    loonggpu_cs_ctx_free(ctx);
  }
  if (device) {
    loonggpu_device_deinitialize(device);
  }
  if (fd >= 0) {
    close(fd);
  }
}

// One submission: the six fixed-address BOs are created (VS from the fixed
// DS-as-VS program, FS from the submitted dwords, packet from the IB,
// argument BO from the preload bytes), submitted once, fence-waited,
// read back, and health-checked (guard canary + context reset/hang state).
PsSubmitReport submit_once(const std::vector<std::uint8_t>& fs_bytes,
                           const std::vector<std::uint32_t>& ib_words,
                           const std::vector<std::uint8_t>& arg_bytes,
                           std::uint32_t timeout_ms,
                           // Which resource holds the timestamp pair, or
                           // kResourceCount for none.  NOT zero-based
                           // "no timestamp": 0 is the argument BO, which is
                           // exactly where the 0x13 writeback lands, so using
                           // 0 as the sentinel silently disabled the readback
                           // for every caller that wanted it.
                           std::uint32_t ts_bo = kResourceCount,
                           std::uint32_t ts_off = 0x3000,
                           const std::vector<std::uint8_t>& gs_bytes = {}) {
  PsSubmitReport report;
  report.status = PsStatus::Rejected;

  int fd = -1;
  loonggpu_device_handle device = nullptr;
  loonggpu_context_handle ctx = nullptr;
  if (!open_session(fd, device, ctx, report.detail)) {
    return report;
  }

  const std::uint64_t rw_flags =
      LOONGGPU_VM_PAGE_READABLE | LOONGGPU_VM_PAGE_WRITEABLE;
  const std::uint64_t exec_flags =
      LOONGGPU_VM_PAGE_READABLE | LOONGGPU_VM_PAGE_EXECUTABLE;

  std::array<Bo, kResourceCount + 1> resources;
  const bool ok =
      create_resource_at(device, resources[0],
                         carrier::PsControlPacket::resource_address(0),
                         rw_flags, arg_bytes.data(), arg_bytes.size(), 0,
                         report.detail) &&
      create_resource_at(device, resources[1],
                         carrier::PsControlPacket::resource_address(1),
                         rw_flags, nullptr, kImageSize, 0xcd, report.detail) &&
      create_resource_at(device, resources[2],
                         carrier::PsControlPacket::resource_address(2),
                         exec_flags, carrier::generated::kPsControlVs,
                         carrier::generated::kPsControlVsSize, 0,
                         report.detail) &&
      create_resource_at(device, resources[3],
                         carrier::PsControlPacket::resource_address(3),
                         exec_flags, fs_bytes.data(), fs_bytes.size(), 0,
                         report.detail) &&
      create_resource_at(device, resources[4],
                         carrier::PsControlPacket::resource_address(4),
                         LOONGGPU_VM_PAGE_READABLE, ib_words.data(),
                         ib_words.size() * sizeof(ib_words[0]), 0,
                         report.detail) &&
      create_resource_at(device, resources[5],
                         carrier::PsControlPacket::resource_address(5),
                         rw_flags, nullptr, 0, 0xa5, report.detail) &&
      (gs_bytes.empty() ||
       create_resource_at(
           device, resources[6],
           carrier::PsControlPacket::stage_program_address(
               carrier::PsControlPacket::ShaderStage::Gs),
           exec_flags, gs_bytes.data(), gs_bytes.size(), 0, report.detail));
  if (!ok) {
    close_session(fd, device, ctx);
    return report;
  }

  std::vector<loonggpu_bo_handle> handles;
  handles.reserve(kResourceCount);
  for (std::size_t ri = 0; ri < kResourceCount; ++ri) {
    handles.push_back(resources[ri].handle);
  }
  loonggpu_bo_list_handle bo_list = nullptr;
  if (loonggpu_bo_list_create(device,
                              static_cast<std::uint32_t>(handles.size()),
                              handles.data(), nullptr, &bo_list) ||
      !bo_list) {
    report.detail = "bo list create failed";
    close_session(fd, device, ctx);
    return report;
  }

  loonggpu_cs_ib_info ib{};
  ib.ib_mc_address = resources[4].address;
  ib.size = static_cast<std::uint32_t>(ib_words.size() * sizeof(ib_words[0]));
  loonggpu_cs_request req{};
  req.context = ctx;
  req.ip_type = LOONGGPU_HW_IP_GFX;
  req.ip_instance = 0;
  req.ring = 0;
  req.resources = bo_list;
  req.number_of_ibs = 1;
  req.ibs = &ib;

  if (loonggpu_cs_submit(device, 0, &req, 1) || !req.seq_no) {
    report.detail = "submit failed";
    loonggpu_bo_list_destroy(bo_list);
    close_session(fd, device, ctx);
    return report;
  }

  loonggpu_cs_fence fence{};
  fence.context = ctx;
  fence.ip_type = LOONGGPU_HW_IP_GFX;
  fence.ip_instance = 0;
  fence.ring = 0;
  fence.fence = req.seq_no;
  std::uint32_t expired = 0;
  const std::uint64_t timeout_ns =
      static_cast<std::uint64_t>(timeout_ms) * 1000000ull;
  const int fence_result =
      loonggpu_cs_query_fence_status(&fence, timeout_ns, 0, &expired);
  if (fence_result || !expired) {
    report.status = PsStatus::Timeout;
    report.detail = "fence timeout";
    loonggpu_bo_list_destroy(bo_list);
    close_session(fd, device, ctx);
    return report;
  }

  // Read back the result region (argument BO).
  const std::uint32_t readback =
      arg_bytes.size() > kPageSize ? kPageSize
                                   : static_cast<std::uint32_t>(arg_bytes.size());
  void* result_map = nullptr;
  if (loonggpu_bo_cpu_map(resources[0].handle, &result_map) || !result_map) {
    report.detail = "result map failed";
    loonggpu_bo_list_destroy(bo_list);
    close_session(fd, device, ctx);
    return report;
  }
  report.result.resize(readback);
  std::memcpy(report.result.data(), result_map, readback);
  loonggpu_bo_cpu_unmap(resources[0].handle);

  // Post-submit health: guard canary + context reset/hang state.
  const bool guard_ok =
      check_bo_pattern(resources[5].handle, 0, kPageSize, 0xa5,
                       report.detail);
  std::uint32_t reset_after = 0, hangs_after = 0;
  const int health =
      loonggpu_cs_query_reset_state(ctx, &reset_after, &hangs_after);
  const bool healthy = !health && !reset_after && !hangs_after;

  if (guard_ok && healthy && ts_bo < kResourceCount) {
    void* ts_map = nullptr;
    if (!loonggpu_bo_cpu_map(resources[ts_bo].handle, &ts_map) && ts_map) {
      std::uint64_t tb = 0, te = 0;
      std::memcpy(&tb, static_cast<const std::uint8_t*>(ts_map) + ts_off, 8);
      std::memcpy(&te, static_cast<const std::uint8_t*>(ts_map) + ts_off + 8,
                  8);
      report.ts_begin = tb;
      report.ts_end = te;
      loonggpu_bo_cpu_unmap(resources[ts_bo].handle);
    }
  }

  loonggpu_bo_list_destroy(bo_list);
  close_session(fd, device, ctx);

  if (!guard_ok || !healthy) {
    report.status = PsStatus::Fault;
    if (report.detail.empty()) {
      report.detail = "GPU fault detected";
    }
    return report;
  }
  report.status = PsStatus::Clean;
  return report;
}

}  // namespace

PsSubmitReport submit_ps(const PsSubmitRequest& request) {
  PsSubmitReport report;
  report.status = PsStatus::Rejected;

  if (request.shader_words.empty()) {
    report.detail = "empty PS program";
    return report;
  }
  const std::uint32_t shader_bytes =
      static_cast<std::uint32_t>(request.shader_words.size()) * 4;
  if (shader_bytes % 16 != 0) {
    report.detail = "PS program is not 16-byte aligned";
    return report;
  }

  // Template roots: the PS shader-info const/constant/rsd stay at their
  // relocated defaults (arg + 0x100/0x100/0x200).
  std::array<std::uint32_t, kPacketDwords> packet;
  const std::uint64_t arg = carrier::kArgVa;
  if (!build_packet(packet, shader_bytes, arg + 0x100, arg + 0x100,
                    arg + 0x200, report.detail)) {
    return report;
  }
  std::vector<std::uint32_t> ib;
  // The 0x13 writeback only lands in the argument BO (resource 0) on this
  // user-IB path, so that is where the pair is read back from.
  std::uint32_t ts_bo = carrier::PsControlPacket::kResourceCount;
  std::uint32_t ts_off = 0x3000;
  if (request.timestamped) {
    ts_bo = 0;
    // Opcode 0x13 wrapper (probe-verified): begin at the IB prefix, end
    // after the end command.  The writeback only lands in the first rw
    // resource (arg BO) on this user-IB path, so bench envelopes with
    // LGPU_BENCH_DST point their result stores at the color BO to keep the
    // arg window exclusive (a same-BO conflict drops the end writeback).
    const std::uint64_t taddr = arg + 0x3000;
    auto ts = [&](std::uint32_t op, std::uint64_t a) {
      ib.push_back(op);
      ib.push_back(static_cast<std::uint32_t>(a));
      ib.push_back(static_cast<std::uint32_t>(a >> 32));
    };
    ts(0x13u | (1u << 8), taddr);
    ib.insert(ib.end(), packet.begin(), packet.end());
    ts(0x13u | (0x200u << 8), taddr + 8);
    while (ib.size() % 4 != 0) ib.push_back(0u);  // 16-byte IB alignment
  } else {
    ib.assign(packet.begin(), packet.end());
  }

  // The generic carrier writes its results through its own store descriptor;
  // the argument BO is zero-preloaded so unwritten windows compare cleanly.
  std::vector<std::uint8_t> arg_bytes(request.result_bytes, 0);
  for (std::size_t i = 0; i < request.argument_dwords.size(); ++i) {
    if (4 * i + 4 > arg_bytes.size()) break;
    const std::uint32_t w = request.argument_dwords[i];
    arg_bytes[4 * i + 0] = static_cast<std::uint8_t>(w);
    arg_bytes[4 * i + 1] = static_cast<std::uint8_t>(w >> 8);
    arg_bytes[4 * i + 2] = static_cast<std::uint8_t>(w >> 16);
    arg_bytes[4 * i + 3] = static_cast<std::uint8_t>(w >> 24);
  }
  return submit_once(words_to_bytes(request.shader_words),
                     ib, arg_bytes, request.timeout_ms, ts_bo, ts_off);
}

PsSubmitReport submit_gs(const PsSubmitRequest& request) {
  PsSubmitReport report;
  report.status = PsStatus::Rejected;

  if (request.shader_words.empty() || request.gs_words.empty()) {
    report.detail = "empty PS/GS program";
    return report;
  }
  const std::uint32_t shader_bytes =
      static_cast<std::uint32_t>(request.shader_words.size()) * 4;
  const std::uint32_t gs_bytes =
      static_cast<std::uint32_t>(request.gs_words.size()) * 4;
  if (shader_bytes % 16 != 0 || gs_bytes % 16 != 0) {
    report.detail = "PS/GS program is not 16-byte aligned";
    return report;
  }

  const std::uint64_t arg = carrier::kArgVa;
  carrier::PsControlPacket p;
  if (!p.patch(report.detail)) {
    return report;
  }
  p.set_fs_code_size(shader_bytes);
  p.set_ps_roots_full(arg + 0x100, arg + 0x100, arg + 0x200);
  p.set_stage_record(
      carrier::PsControlPacket::ShaderStage::Gs, 4, gs_bytes, 8,
      carrier::PsControlPacket::stage_program_address(
          carrier::PsControlPacket::ShaderStage::Gs),
      0x100000100ull, 0x100000456ull, 0x100000a00ull);
  // probe-proven GS stage config: kShaderConfigGsEnable = 1<<3 only;
  // the template baseline (0x24 DS-as-VS) is preserved as-is.
  p.set_stage_config(1u << 3);


  std::array<std::uint32_t, kPacketDwords> packet;
  std::memcpy(packet.data(), p.data(), sizeof(packet));
  if (std::getenv("LGPU_DUMP_PKT")) {
    std::fprintf(stderr, "pkt 4cc=%08x gs0=%08x gs1=%08x\n",
                 packet[0x4cc / 4],
                 packet[(0x488 + 0x0c) / 4], packet[(0x488 + 0x10) / 4]);
  }
  if (const char* dp = std::getenv("LGGPU_DUMP_PACKET")) {
    std::FILE* df = std::fopen(dp, "wb");
    if (df) {
      std::fwrite(packet.data(), 4, kPacketDwords, df);
      std::fclose(df);
    }
  }
  std::vector<std::uint32_t> ib(packet.begin(), packet.end());

  std::vector<std::uint8_t> arg_bytes(request.result_bytes, 0);
  for (std::size_t i = 0; i < request.argument_dwords.size(); ++i) {
    if (4 * i + 4 > arg_bytes.size()) break;
    const std::uint32_t w = request.argument_dwords[i];
    arg_bytes[4 * i + 0] = static_cast<std::uint8_t>(w);
    arg_bytes[4 * i + 1] = static_cast<std::uint8_t>(w >> 8);
    arg_bytes[4 * i + 2] = static_cast<std::uint8_t>(w >> 16);
    arg_bytes[4 * i + 3] = static_cast<std::uint8_t>(w >> 24);
  }
  return submit_once(words_to_bytes(request.shader_words), ib, arg_bytes,
                     request.timeout_ms, carrier::PsControlPacket::kResourceCount,
                     0x3000, words_to_bytes(request.gs_words));
}

PsSubmitReport submit_probe(
    const std::vector<std::uint32_t>& shader_words,
    const std::vector<std::pair<std::uint32_t, std::uint32_t>>& arg_preload,
    std::uint64_t const_buffer, std::uint64_t constant, std::uint64_t rsd,
    std::uint32_t result_bytes, std::uint32_t timeout_ms) {
  PsSubmitReport report;
  report.status = PsStatus::Rejected;
  if (shader_words.empty()) {
    report.detail = "empty probe program";
    return report;
  }
  const std::uint32_t shader_bytes =
      static_cast<std::uint32_t>(shader_words.size()) * 4;
  if (shader_bytes % 16 != 0) {
    report.detail = "probe program not 16-byte aligned";
    return report;
  }
  std::array<std::uint32_t, kPacketDwords> packet;
  if (!build_packet(packet, shader_bytes, const_buffer, constant, rsd,
                    report.detail)) {
    return report;
  }
  // Argument BO from sparse (byte-offset, dword) preload pairs (RSD table,
  // load window, texel rows, ...), zero otherwise, clamped to one page.
  std::vector<std::uint8_t> arg_bytes(result_bytes, 0);
  for (const auto& entry : arg_preload) {
    const std::uint32_t off = entry.first;
    const std::uint32_t value = entry.second;
    if (off + 4 > arg_bytes.size()) {
      arg_bytes.resize(off + 4, 0);
    }
    arg_bytes[off + 0] = static_cast<std::uint8_t>(value & 0xff);
    arg_bytes[off + 1] = static_cast<std::uint8_t>((value >> 8) & 0xff);
    arg_bytes[off + 2] = static_cast<std::uint8_t>((value >> 16) & 0xff);
    arg_bytes[off + 3] = static_cast<std::uint8_t>((value >> 24) & 0xff);
  }
  if (arg_bytes.size() > kPageSize) arg_bytes.resize(kPageSize);
  return submit_once(words_to_bytes(shader_words),
                     std::vector<std::uint32_t>(packet.begin(), packet.end()),
                     arg_bytes, timeout_ms);
}


PsSubmitReport submit_timestamp_probe(std::uint32_t shape,
                                      std::uint32_t timeout_ms,
                                      std::uint32_t fs_size) {
  // Timestamp-command probe (experiment): opcode 0x13 begin at the IB prefix
  // and end after the end command (the reference captures place them at
  // offset 0x0000 / 0x081c).  Results land at arg+0x3000/0x3008.  Verified:
  // both write back with the default FS; with carrier-style FS programs the
  // END writeback is dropped when the program stores to the arg BO (same-BO
  // effect) and color-BO destinations are not accepted on this path.
  PsSubmitReport report;
  report.status = PsStatus::Rejected;
  const std::uint64_t addr = carrier::kArgVa + 0x3000;
  std::vector<std::uint32_t> ib;
  auto append_ts = [&](std::uint32_t op, std::uint64_t a) {
    ib.push_back(op);
    ib.push_back(static_cast<std::uint32_t>(a));
    ib.push_back(static_cast<std::uint32_t>(a >> 32));
  };
  append_ts(0x13u | (1u << 8), addr);
  // Insert the state object for every shape except the bare pair.  Restricting
  // this to shapes 1 and 3 made the shape-7/8 experiments meaningless: without
  // a state object the IB is only the two timestamps, so their "delta" could
  // only ever be the pair overhead and said nothing about the program under it.
  if (shape != 0 && shape != 2) {
    std::array<std::uint32_t, kPacketDwords> packet;
    std::uint32_t code = fs_size ? fs_size : carrier::generated::kPsControlFsSize;
    if (shape == 3) code = 192;  // hybrid: default FS + 8x v_add inserted
    if (!build_packet(packet, code,
                      carrier::kArgVa + 0x100, carrier::kArgVa + 0x100,
                      carrier::kArgVa + 0x200, report.detail)) {
      return report;
    }
    ib.insert(ib.end(), packet.begin(), packet.end());
  }
  append_ts(0x13u | (0x200u << 8), addr + 8);
  while (ib.size() % 4 != 0) ib.push_back(0u);
  std::vector<std::uint8_t> fs;
  if (shape == 1) {
    fs.assign(std::begin(carrier::generated::kPsControlFs),
              std::end(carrier::generated::kPsControlFs));
  } else if (shape == 7) {
    // default FS body WITHOUT the RT-export block (bytes 56-95) + tail
    const auto base = carrier::generated::kPsControlFs;
    fs.insert(fs.end(), base, base + 56);
    fs.insert(fs.end(), base + 96, base + 128);
  } else if (shape == 8) {
    // carrier-like program + the default FS export block before the tail:
    // the export makes the render target complete, which is the suspected
    // drain gate for the 0x13 END writeback.
    static const std::uint8_t kDesc[32] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x40,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x17, 0x40,
        0x40, 0x00, 0x00, 0x00, 0x00, 0x02, 0x17, 0x40,
        0xac, 0x7f, 0x02, 0x00, 0x00, 0x03, 0x17, 0x40};
    static const std::uint8_t kVcc[8] = {0x00, 0x00, 0x00, 0x00,
                                         0x00, 0x06, 0x17, 0x40};
    static const std::uint8_t kV9[8] = {0x66, 0x00, 0x00, 0x09,
                                        0x00, 0x80, 0x09, 0x10};
    static const std::uint8_t kAdd[8] = {0x00, 0x01, 0x06, 0x0a,
                                         0x09, 0x00, 0x00, 0x10};
    const auto base = carrier::generated::kPsControlFs;
    fs.insert(fs.end(), kDesc, kDesc + 32);
    fs.insert(fs.end(), kVcc, kVcc + 8);
    fs.insert(fs.end(), kV9, kV9 + 8);
    const int copies = fs_size ? static_cast<int>(fs_size) : 8;
    for (int i = 0; i < copies; ++i)
      fs.insert(fs.end(), kAdd, kAdd + 8);
    fs.insert(fs.end(), base + 56, base + 96);  // export block
    fs.insert(fs.end(), base + 104, base + 128);  // check/nop/exit tail
  } else if (shape >= 3 && shape <= 6) {
    // FS-body bisection: default FS body with a candidate carrier-prelude
    // instruction class spliced before the review check/nop/exit tail.
    static const std::uint8_t kAdd[8] = {0x00, 0x01, 0x06, 0x0a,
                                         0x09, 0x00, 0x00, 0x10};
    static const std::uint8_t kDesc[32] = {  // s_mov_imm s0-s3 descriptor
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x40,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x17, 0x40,
        0x40, 0x00, 0x00, 0x00, 0x00, 0x02, 0x17, 0x40,
        0xac, 0x7f, 0x02, 0x00, 0x00, 0x03, 0x17, 0x40};
    static const std::uint8_t kVcc[8] = {0x00, 0x00, 0x00, 0x00,
                                         0x00, 0x06, 0x17, 0x40};
    static const std::uint8_t kV9[8] = {0x66, 0x00, 0x00, 0x09,
                                        0x00, 0x80, 0x09, 0x10};
    const auto base = carrier::generated::kPsControlFs;
    fs.insert(fs.end(), base, base + 96);   // default body (12 instr)
    if (shape == 3) {
      for (int i = 0; i < 8; ++i)
        fs.insert(fs.end(), kAdd, kAdd + 8);  // 8x v_add_u32
    } else if (shape == 4) {
      fs.insert(fs.end(), kDesc, kDesc + 32);  // buff store descriptor
    } else if (shape == 5) {
      fs.insert(fs.end(), kVcc, kVcc + 8);     // s_mov_imm vcc(6)
    } else if (shape == 6) {
      fs.insert(fs.end(), kV9, kV9 + 8);       // v_mov p0, v9, zero
    }
    fs.insert(fs.end(), base + 104, base + 128);  // check/nop/exit tail
  }
  std::vector<std::uint8_t> arg(kPageSize, 0);
  // No ts_bo: this path reads the pair out of report.result itself, at the
  // same offset, and always has.
  return submit_once(fs, ib, arg, timeout_ms,
                     carrier::PsControlPacket::kResourceCount, 0x3000);
}

ComputeQueueReport compute_queue_probe(std::uint32_t ring_size) {
  ComputeQueueReport report;

  // Kernel-side bounds, from LOONGGPU_USERQ_MIN_SIZE / _MAX_SIZE.  Checked
  // here rather than letting the ioctl reject it, so the caller learns which
  // constraint it broke.
  if (ring_size < 4096u || ring_size > (1u << 20)) {
    report.detail = "ring size outside LOONGGPU_USERQ_MIN_SIZE.._MAX_SIZE";
    return report;
  }
  // A stream is published by a byte write offset that must be 64-byte aligned,
  // so the ring it indexes has to be a multiple of 64.
  if (ring_size % 64u != 0) {
    report.detail = "ring size is not a multiple of 64";
    return report;
  }

  int fd = -1;
  loonggpu_device_handle device = nullptr;
  loonggpu_context_handle ctx = nullptr;
  if (!open_session(fd, device, ctx, report.detail)) {
    return report;
  }

  // One BO holds both the index block and the ring.  The index block must sit
  // immediately BELOW the ring: the firmware reads the write index at
  // queue_va - LOONGGPU_USERQ_INDEX_SIZE, not inside the ring.  Putting the
  // two in one mapping keeps that relationship true by construction.
  const std::uint64_t index_size = 64u;
  // Round the BO up to whole pages.  The VA mapping is page-granular, and a
  // buffer whose size is not a page multiple is rejected by
  // loonggpu_bo_va_op - the index block plus a 64 KiB ring is 65600 bytes,
  // which is 4.003 pages.
  const std::uint64_t total =
      ((index_size + ring_size + kPageSize - 1) / kPageSize) * kPageSize;
  const std::uint64_t base_va = kCsRingBaseVa;

  loonggpu_bo_handle bo = nullptr;
  loonggpu_bo_alloc_request request{};
  request.alloc_size = total;
  request.phys_alignment = kPageSize;
  request.preferred_heap = LOONGGPU_GEM_DOMAIN_GTT;
  request.flags = LOONGGPU_GEM_CREATE_CPU_ACCESS_REQUIRED;
  if (loonggpu_bo_alloc(device, &request, &bo) || !bo) {
    report.detail = "ring BO allocation failed";
    close_session(fd, device, ctx);
    return report;
  }

  void* map = nullptr;
  if (loonggpu_bo_cpu_map(bo, &map) || !map) {
    report.detail = "ring BO cpu map failed";
    loonggpu_bo_free(bo);
    close_session(fd, device, ctx);
    return report;
  }
  // Zero it: the write index must start at 0 so the first published offset is
  // the stream's own end and not whatever the page happened to hold.
  std::memset(map, 0, total);
  loonggpu_bo_cpu_unmap(bo);

  if (loonggpu_bo_va_op(bo, 0, total, base_va, 0, LOONGGPU_VA_OP_MAP)) {
    report.detail = "ring BO VA map failed";
    loonggpu_bo_free(bo);
    close_session(fd, device, ctx);
    return report;
  }

  const std::uint64_t queue_va = base_va + index_size;
  std::uint32_t queue_id = 0;
  const int rc = loonggpu_userq_create(device, LOONGGPU_HW_IP_COMPUTE, queue_va,
                                       ring_size, &queue_id);

  // Tear the queue down before the mapping it was created over, and report the
  // geometry either way so a rejected create still says what was asked for.
  report.ring_va = queue_va;
  report.ring_size = ring_size;
  if (rc != 0) {
    report.detail = "loonggpu_userq_create(COMPUTE) failed";
    loonggpu_bo_va_op(bo, 0, total, base_va, 0, LOONGGPU_VA_OP_UNMAP);
    loonggpu_bo_free(bo);
    close_session(fd, device, ctx);
    return report;
  }

  report.queue_id = queue_id;
  report.ok = true;

  loonggpu_userq_free(device, queue_id);
  loonggpu_bo_va_op(bo, 0, total, base_va, 0, LOONGGPU_VA_OP_UNMAP);
  loonggpu_bo_free(bo);
  close_session(fd, device, ctx);
  return report;
}

ComputePlanReport compute_plan(std::uint64_t text_size, std::uint32_t group_x,
                               std::uint32_t group_y, std::uint32_t group_z,
                               std::uint32_t grid_x, std::uint32_t grid_y,
                               std::uint32_t grid_z) {
  ComputePlanReport report;
  (void)text_size;  // only sizes the caller's BO; the stream carries the VA

  loongvk_mcs_dispatch d;
  make_cs_dispatch(d, group_x, group_y, group_z, grid_x, grid_y, grid_z);

  // The descriptor first: patch_kd rejects a zero group dimension, and
  // reporting that here is more useful than a build_dispatch failure later.
  report.kd.resize(LOONGVK_KD_SIZE);
  if (!loongvk_mcs_patch_kd(report.kd.data(), report.kd.size(), &d)) {
    report.detail =
        "loongvk_mcs_patch_kd failed (zero group dimension, or short buffer)";
    return report;
  }
  // patch_kd writes only the PER-DISPATCH fields.  The .kd +0x00 flag word is
  // set once, when the descriptor is created (LoongVK does it in
  // loongvk_kd_build, which is not vendored here because it drags in
  // lco_abi.h).  We build the descriptor from nothing, so we set it.  Leaving
  // it zero is silent: the stream still lists the configuration commands
  // because those come from d.kd_flags, but the descriptor the firmware reads
  // would not enable the same SGPRs.
  const std::uint32_t kd_flags_le = LOONGVK_KD_FLAGS;
  std::memcpy(report.kd.data() + 0, &kd_flags_le, sizeof kd_flags_le);

  // LOONGVK_MCS_MAX_LEN is the worst-case stream the ring slot must fit, so
  // build into a buffer that size and then keep only what was written.
  std::vector<std::uint8_t> out(LOONGVK_MCS_MAX_LEN);
  std::size_t len = 0;
  if (!loongvk_mcs_build_dispatch(out.data(), out.size(), &d, &len)) {
    report.detail = "loongvk_mcs_build_dispatch failed";
    return report;
  }
  report.stream.assign(out.begin(), out.begin() + static_cast<std::ptrdiff_t>(len));

  report.text_va = kCsTextVa;
  report.kd_va = kCsKdVa;
  report.kernarg_va = kCsKernargVa;
  report.signal_va = kCsSignalVa;
  report.num_sgprs = kCsNumSgprs;
  report.num_vgprs = kCsNumVgprs;
  report.ok = true;
  return report;
}

ComputeSubmitReport submit_compute(const ComputeSubmitRequest& request) {
  ComputeSubmitReport report;
  if (request.text.empty()) {
    report.detail = "empty kernel text";
    return report;
  }
  if (request.output_size == 0) {
    report.detail = "zero output size";
    return report;
  }
  for (int i = 0; i < 3; ++i) {
    if (request.group[i] == 0) {
      report.detail = "zero group dimension";
      return report;
    }
  }

  int fd = -1;
  loonggpu_device_handle device = nullptr;
  loonggpu_context_handle ctx = nullptr;
  if (!open_session(fd, device, ctx, report.detail)) {
    return report;
  }

  // Every buffer is a fixed-address BO, so their destructors unmap and free
  // them in reverse order of declaration.  The ring outlives the queue by
  // construction: the queue is freed explicitly below, before this scope ends.
  Bo ring, text, kd, kernarg, output, signal;
  const std::uint64_t ring_total =
      ((64u + kCsRingSize + kPageSize - 1) / kPageSize) * kPageSize;

  std::uint32_t qid = 0;
  bool queue_created = false;
  auto fail = [&](const char* what) {
    if (queue_created) {
      loonggpu_userq_free(device, qid);
    }
    report.detail = what;
    return report;
  };

  if (!alloc_bo_at(device, ring, kCsRingBaseVa, ring_total, 0, report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }
  if (!alloc_bo_at(device, text, kCsTextVa, request.text.size(), 0,
                   report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }
  if (!alloc_bo_at(device, kd, kCsKdVa, LOONGVK_KD_SIZE, 0, report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }
  // The kernarg needs room for the eight-byte output pointer plus whatever the
  // caller supplied.
  const std::uint64_t kernarg_bytes = 8u + request.kernarg_tail.size();
  if (!alloc_bo_at(device, kernarg, kCsKernargVa, kernarg_bytes, 0,
                   report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }
  if (!alloc_bo_at(device, output, kCsOutputVa, request.output_size, 0,
                   report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }
  // Arm the completion signal at zero.  A completed dispatch leaves
  // 0xFFFFFFFFFFFFFFFF behind (0 - 1), which is why the wait below tests for
  // that value rather than for anything below a threshold.
  // The completion counter is at offset 0, but the vendor's own OpenCL
  // profiling gets START/END timestamps for compute dispatches from this same
  // firmware queue, so the signal allocation is made large enough that a
  // firmware-written timestamp block would land inside it and be visible.
  if (!alloc_bo_at(device, signal, kCsSignalVa, kCsSignalBytes, 0,
                   report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }

  if (!poke_bo(text.handle, 0, request.text.data(), request.text.size(),
               report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }

  // Kernel descriptor: build it with the plan path so the host-side dump and
  // the real submit cannot drift.
  const ComputePlanReport plan =
      compute_plan(request.text.size(), request.group[0], request.group[1],
                   request.group[2], request.grid[0], request.grid[1],
                   request.grid[2]);
  if (!plan.ok) {
    fail(plan.detail.c_str());
    close_session(fd, device, ctx);
    return report;
  }
  if (!poke_bo(kd.handle, 0, plan.kd.data(), plan.kd.size(), report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }

  // Kernarg: the output pointer first, then the caller's tail.  This is the
  // layout the compiled kernel expects - its first instruction loads exactly
  // these eight bytes through s[6:7].
  {
    std::vector<std::uint8_t> ka(kernarg_bytes, 0);
    const std::uint64_t out_va = kCsOutputVa;
    std::memcpy(ka.data(), &out_va, sizeof out_va);
    if (!request.kernarg_tail.empty()) {
      std::memcpy(ka.data() + 8, request.kernarg_tail.data(),
                  request.kernarg_tail.size());
    }
    if (!poke_bo(kernarg.handle, 0, ka.data(), ka.size(), report.detail)) {
      close_session(fd, device, ctx);
      return report;
    }
  }

  // The stream goes in at ring offset 0 and its END is what gets published.
  if (!poke_bo(ring.handle, 64, plan.stream.data(), plan.stream.size(),
               report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }
  const std::uint64_t wptr = plan.stream.size();
  if (!poke_bo(ring.handle, 0, &wptr, sizeof wptr, report.detail)) {
    close_session(fd, device, ctx);
    return report;
  }

  const std::uint64_t queue_va = kCsRingBaseVa + 64u;
  if (loonggpu_userq_create(device, LOONGGPU_HW_IP_COMPUTE, queue_va,
                            kCsRingSize, &qid) != 0) {
    close_session(fd, device, ctx);
    return fail("loonggpu_userq_create(COMPUTE) failed");
  }
  queue_created = true;

  if (loonggpu_userq_doorbell(device, qid, wptr) != 0) {
    close_session(fd, device, ctx);
    return fail("loonggpu_userq_doorbell failed");
  }

  // Poll the completion signal.  Map once and read through the mapping; the
  // device writes it, so every read has to come from memory rather than a
  // register copy.
  {
    void* sig_map = nullptr;
    if (loonggpu_bo_cpu_map(signal.handle, &sig_map) || !sig_map) {
      close_session(fd, device, ctx);
      return fail("signal BO map failed");
    }
    const volatile std::uint64_t* sig =
        static_cast<const volatile std::uint64_t*>(sig_map);
    const auto deadline =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(request.timeout_ms);
    std::uint64_t seen = 0;
    for (;;) {
      std::this_thread::sleep_for(std::chrono::microseconds(200));
      seen = *sig;
      if (seen != 0) {
        break;
      }
      if (std::chrono::steady_clock::now() >= deadline) {
        break;
      }
    }
    if (request.dump_signal) {
      report.signal_raw.assign(
          static_cast<const std::uint8_t*>(sig_map),
          static_cast<const std::uint8_t*>(sig_map) + kCsSignalBytes);
    }
    loonggpu_bo_cpu_unmap(signal.handle);
    report.signal_value = seen;
    if (seen == 0) {
      close_session(fd, device, ctx);
      return fail("completion signal never retired (timeout)");
    }
  }

  // Read the output back.
  {
    void* out_map = nullptr;
    if (loonggpu_bo_cpu_map(output.handle, &out_map) || !out_map) {
      close_session(fd, device, ctx);
      return fail("output BO map failed");
    }
    report.output.assign(static_cast<const std::uint8_t*>(out_map),
                         static_cast<const std::uint8_t*>(out_map) +
                             request.output_size);
    loonggpu_bo_cpu_unmap(output.handle);
  }

  loonggpu_userq_free(device, qid);
  queue_created = false;

  report.status = (report.signal_value == 0xffffffffffffffffull)
                      ? ComputeSubmitReport::Status::Clean
                      : ComputeSubmitReport::Status::Fault;
  if (report.status == ComputeSubmitReport::Status::Fault) {
    report.detail = "completion signal retired with an unexpected value";
  }
  close_session(fd, device, ctx);
  return report;
}

#else  // !LGPU_HAVE_LIBDRM

PsSubmitReport submit_ps(const PsSubmitRequest& request) {
  (void)request;
  PsSubmitReport report;
  report.status = PsStatus::Rejected;
  report.detail = "no libdrm_loonggpu; dry-run only";
  return report;
}

ComputeQueueReport compute_queue_probe(std::uint32_t ring_size) {
  (void)ring_size;
  ComputeQueueReport report;
  report.detail = "no libdrm_loonggpu; dry-run only";
  return report;
}

ComputePlanReport compute_plan(std::uint64_t text_size, std::uint32_t group_x,
                               std::uint32_t group_y, std::uint32_t group_z,
                               std::uint32_t grid_x, std::uint32_t grid_y,
                               std::uint32_t grid_z) {
  (void)text_size;
  (void)group_x; (void)group_y; (void)group_z;
  (void)grid_x; (void)grid_y; (void)grid_z;
  ComputePlanReport report;
  report.detail = "no libdrm_loonggpu; dry-run only";
  return report;
}

ComputeSubmitReport submit_compute(const ComputeSubmitRequest& request) {
  (void)request;
  ComputeSubmitReport report;
  report.detail = "no libdrm_loonggpu; dry-run only";
  return report;
}

#endif  // LGPU_HAVE_LIBDRM

}  // namespace lgpu::drm