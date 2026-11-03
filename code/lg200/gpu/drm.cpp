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

#include <algorithm>
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
//
// Order within the block matters: the text must be at a page start and the
// kernarg and output are named by constants that the assembled program also
// carries, so the layout is published through compute_va_layout() rather than
// repeated.  Nothing may land between these and the PS block below
// 0x100018000, and fixed-address mapping means a collision would not be
// detected until the VA map failed.
constexpr std::uint64_t kCsRingBaseVa = carrier::kCsRingBaseVa;
constexpr std::uint64_t kCsTextVa = carrier::kCsTextVa;
constexpr std::uint64_t kCsKdVa = carrier::kCsKdVa;
constexpr std::uint64_t kCsKernargVa = carrier::kCsKernargVa;
constexpr std::uint64_t kCsOutputVa = carrier::kCsArgVa;
constexpr std::uint64_t kCsSignalVa = carrier::kCsSignalVa;

// Register counts declared in rsrc1 and the .kd.  LOONGVK_KD_RSRC1 already
// carries kernel_sgpr_cnt = 11, which is where the hardware writes the three
// GROUP ID registers (s11/s12/s13); the granulated counts below are only the
// allocation sizes.  The caller passes the emitter's real high-water mark, so
// these defaults exist for compute_plan's host-side dump and for the probe.
constexpr std::uint32_t kCsDefaultSgprs = 32;
constexpr std::uint32_t kCsDefaultVgprs = 32;

// The CPIPE ring.  LoongVK uses 64 KiB; the kernel's own bounds are 4 KiB to
// 1 MiB, and a stream is at most LOONGVK_MCS_MAX_LEN, so this is generous
// rather than tight - the ring only ever holds one stream at a time here.
constexpr std::uint32_t kCsRingSize = 64u * 1024u;
// The compute kernarg block is one page, the same window budget the graphics
// argument BO gives a carrier: preloads address their own windows by offset
// (resource tuples at 0x200/0x230, load window 0x300, texel window 0x400).
constexpr std::uint64_t kCsKernargBytes = 0x2000;
constexpr std::uint32_t kTrampolineWords[4] = {
    0x00000000u, 0x40300000u,   // nop
    0x00000000u, 0x402e0000u,   // exit
};

// The completion signal allocation.  Only the first eight bytes are the counter
// the carrier waits on; the rest exists so that a firmware-written timestamp
// block, if one is written at all, falls inside a mapping we can read.
constexpr std::uint32_t kCsSignalBytes = 4096u;

// The private-buffer descriptor the vendor hardcodes for a dispatch with no
// scratch region.  An input, not something the builder derives: its last dword
// carries the .kd flag bits in its low half.
constexpr std::uint32_t kCsNoScratchDescriptor[4] = { 0u, 0x80000000u, 0u,
                                                      0x00ca4facu };

// The floating-point denorm mode belongs to the compute dispatch: the graphics
// pipe flushes natively and has no field for it, while the compute pipe selects
// it through rsrc1 bits 14:21 (four 2-bit fields: round32, round16_64,
// denorm32, denorm16_64).  The u-l-i-g oracle's expectations were taken on the
// graphics pipe, so a compute dispatch must request the same behaviour or the
// same instruction reads differently on the two pipes.  This tree's pinned
// loongvk_kd.h predates LOONGVK_KD_FLOAT_MODE_FLUSH, so the word is spelled
// from the layout the header documents: DEFAULT with both denorm fields
// (bits 18:21) cleared to FLUSH_SOURCE_OUTPUT, rounding left nearest-even.
constexpr std::uint32_t kFloatModeFlush =
    LOONGVK_KD_FLOAT_MODE_DEFAULT & ~0x003C0000u;

// Everything one compute dispatch tells the builder.  A struct rather than a
// long parameter list because the host-side plan and the real submit must
// agree on all of it, and a positional call is how the two drift.
struct CsLaunch {
  std::uint32_t group[3] = { 1u, 1u, 1u };
  // GLOBAL size in work-items, not a workgroup count: the group command
  // divides it by the local size (LG200 divides; LG210 rounds up), so a caller
  // holding a workgroup count multiplies it out first.
  std::uint32_t grid[3] = { 1u, 1u, 1u };
  std::uint32_t num_sgprs = kCsDefaultSgprs;
  std::uint32_t num_vgprs = kCsDefaultVgprs;
  std::uint32_t shared_bytes = 0;
};

// Fills a loongvk_mcs_dispatch for the carrier's fixed layout.  Single place
// where the VA constants and the descriptor defaults meet, so the host-side
// plan and the real submit cannot disagree about them.
void make_cs_dispatch(loongvk_mcs_dispatch& d, const CsLaunch& l) {
  d = loongvk_mcs_dispatch{};
  d.kd_flags = LOONGVK_KD_FLAGS;
  d.kernel_object = kCsKdVa;
  d.entry = kCsTextVa;
  d.kernarg_address = kCsKernargVa;
  d.completion_signal = kCsSignalVa;
  d.num_sgprs = l.num_sgprs;
  d.num_vgprs = l.num_vgprs;
  d.shared_segment_size = l.shared_bytes;
  d.group_size[0] = static_cast<std::uint16_t>(l.group[0]);
  d.group_size[1] = static_cast<std::uint16_t>(l.group[1]);
  d.group_size[2] = static_cast<std::uint16_t>(l.group[2]);
  d.grid_size[0] = l.grid[0];
  d.grid_size[1] = l.grid[1];
  d.grid_size[2] = l.grid[2];
  d.group_aperture_hi = 0x00010000u;
  d.private_aperture_hi = 0x00020000u;
  for (int i = 0; i < 4; ++i) {
    d.scratch_resource_descriptor[i] = kCsNoScratchDescriptor[i];
  }
  // [HW 2026-10-12] FLUSH_SOURCE_OUTPUT, not the vendor capture's FLUSH_NONE.
  // The graphics pipe flushes denormals natively and the u-l-i-g oracle's
  // expectations were taken there, so a compute dispatch that leaves this at
  // DEFAULT makes the same instruction disagree with itself between the two
  // pipes.  The vendor's own loongvk_mcs.c names this as the reason "every
  // fp32/fp64 denorm case failed" on the compute path; the compute pipe is the
  // only one that can select the mode.  See with_carrier_float_mode below for
  // the single word this intentionally changes in the capture.
  d.float_mode = kFloatModeFlush;
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
  // Deliberate leak.  A queue that failed to tear down may still be owned by
  // the firmware, and releasing its ring's VM mapping would let CPIPE
  // dereference a recycled address; LoongVK reaches the same conclusion and
  // keeps the allocation until the DRM file is closed, which is the kernel's
  // documented path for a queue that could not be unmapped.  This flag says
  // "leave it mapped and do not free it" instead of doing that by accident.
  bool leaked = false;

  ~Bo() {
    if (leaked) return;
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
//
// `vm_flags` is the LOONGGPU_VM_PAGE_* set the mapping is created with.  The
// graphics carrier's `create_resource_at` takes the same parameter, so the two
// paths state the same intent for the same kind of buffer (code readable +
// executable, data readable + writable) instead of one of them asking for no
// permissions at all.
bool alloc_bo_at(loonggpu_device_handle device, Bo& bo, std::uint64_t va,
                 std::uint64_t bytes, std::uint64_t vm_flags, std::uint8_t fill,
                 std::string& error) {
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
  if (loonggpu_bo_va_op(bo.handle, 0, size, va, vm_flags,
                        LOONGGPU_VA_OP_MAP)) {
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

// ---- session lifetime ----------------------------------------------------
//
// A submission otherwise opens a DRM file, creates a user queue, tears both
// down and closes the file - every time.  The kernel's teardown path
// (postclose -> loonggpu_drm_userq_fini -> loonggpu_lgkcd_userq_destroy) is
// where a few hundred of those cycles cost a power cycle: one teardown fails to
// confirm the queue drop, the DRM VM is retained, and the command processor
// then faults at the ring address it can no longer read.  Measured on
// compute after ~330 cycles: `addr 0x100020000 domain 4` - the queue's own
// index block, which no carrier addresses - followed by
// `compute queue unmap failure` / `retaining DRM VM`.
//
// A bulk caller therefore asks for a PROCESS-LIFETIME session and pays one
// create/destroy pair for the whole batch.  A one-shot caller keeps the
// per-submission session and its isolation.
struct CachedSession {
  int fd = -1;
  loonggpu_device_handle device = nullptr;
  loonggpu_context_handle ctx = nullptr;
  bool open = false;

  // The compute queue and its ring are process-lifetime too, and that is the
  // whole point of the persistent session: the reference driver (mesa loongvk,
  // loongvk_compute.c "struct loongvk_cs_queue") creates ONE queue per logical
  // device and reuses a slot inside a persistent ring.  Creating and destroying
  // a queue per submission instead churns three things at once - the ring BO's
  // VA mapping at kCsRingBaseVa, the PASID-filtered UNMAP_QUEUE
  // (kcd_userq.c: KCD_UNMAP_QUEUES_FILTER_BY_PASID), and the all-VMID TLB flush
  // the doorbell then has to do (loonggpu_drm_userq.c, the K39 block) - and the
  // kernel's own CREATE comment names the resulting shape: "a DRM queue
  // executed in vmid 4 with nothing mapped at its ring address" (the K16
  // wedge).  Measured here: fault addr 0x100020000 (the ring/index block),
  // vmid 4, after ~330 create/destroy cycles.
  Bo ring;
  bool ring_open = false;
  // The rest of the dispatch's fixed-address buffers, cached for the same
  // reason as the ring: CPIPE can still reference a fixed VA after the frontend
  // has dropped its mapping, and the two measured faults are exactly that - the
  // ring VA 0x100020000 first, then a text-region VA 0x100084000 once the ring
  // stopped moving.  A persistent session allocates each of these once.
  Bo text;
  Bo kd;
  Bo kernarg;
  Bo output;
  Bo signal;
  Bo tramp;
  bool text_open = false;
  bool kd_open = false;
  bool kernarg_open = false;
  bool output_open = false;
  bool signal_open = false;
  bool tramp_open = false;
  std::uint32_t qid = 0;
  bool queue_open = false;

  // Next stream slot, in bytes from the ring start; 64-byte aligned.  The
  // write pointer published to the firmware is slot + stream length, exactly
  // loongvk's q->slot + mcs_len.
  std::uint32_t slot = 0;
};

// The ring's write-pointer advance, taken from the reference driver
// (loongvk_mcs.h loongvk_mcs_next_offset): step past the 64-byte-aligned span
// the stream occupied.  The pointer itself is NEVER wrapped in place - a
// backwards write pointer is refused by the firmware - so a full ring recycles
// the queue instead; see the recycle block in submit_compute.
std::uint32_t cs_ring_next_offset(std::uint32_t slot, std::size_t len) {
  return slot + static_cast<std::uint32_t>((len + 63u) & ~std::size_t(63u));
}

CachedSession& cached_session() {
  static CachedSession s;
  return s;
}

bool acquire_session(bool persistent, int& fd, loonggpu_device_handle& device,
                     loonggpu_context_handle& ctx, std::string& error) {
  if (!persistent) return open_session(fd, device, ctx, error);
  CachedSession& s = cached_session();
  if (!s.open) {
    if (!open_session(s.fd, s.device, s.ctx, error)) return false;
    s.open = true;
  }
  fd = s.fd;
  device = s.device;
  ctx = s.ctx;
  return true;
}

void release_session(bool persistent, int fd, loonggpu_device_handle device,
                     loonggpu_context_handle ctx) {
  if (persistent) return;  // owned by the process, not by this submission
  close_session(fd, device, ctx);
}

// Wait until the command processor has consumed the stream this submission
// published.  The ring's index block is the same pair the kernel's own stall
// observer reads: the client's write index at +0 and the firmware's read index
// at +8, both relative to the ring area that starts at +64.  They agree only
// once the CP has fetched the whole stream, which is the precondition for
// tearing the queue down - freeing it earlier is what leaves a firmware queue
// slot pointing at an address space the file is about to drop.
bool ring_drained(loonggpu_bo_handle ring, std::uint64_t wptr,
                  unsigned timeout_ms, std::string& error) {
  void* map = nullptr;
  if (loonggpu_bo_cpu_map(ring, &map) || !map) {
    error = "BO map for drain check failed";
    return false;
  }
  // 200us polls, so a busy timeout budget does not busy-wait the CPU hard.
  const unsigned tries = timeout_ms * 5u + 1u;
  bool drained = false;
  for (unsigned i = 0; i < tries; ++i) {
    std::uint64_t rptr = 0;
    std::memcpy(&rptr, static_cast<const std::uint8_t*>(map) + 8,
                sizeof rptr);
    if (rptr >= wptr) {
      drained = true;
      break;
    }
    usleep(200);
  }
  loonggpu_bo_cpu_unmap(ring);
  if (!drained)
    error = "the command processor never consumed the whole stream";
  return drained;
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

  Bo tramp;  // the indirect-transfer stub, see carrier::kTrampolineVa
  std::array<Bo, kResourceCount + 1> resources;
  const bool ok =
      create_resource_at(device, resources[0],
                         carrier::PsControlPacket::resource_address(0),
                         rw_flags, arg_bytes.data(), arg_bytes.size(), 0,
                         report.detail) &&
      create_resource_at(device, tramp, carrier::kTrampolineVa, exec_flags,
                         kTrampolineWords, sizeof kTrampolineWords, 0,
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

// Close a persistent session in the right order: drain the ring, free the
// ONE queue, then close the DRM file.  Freeing the queue here rather than
// letting postclose do it keeps the fence-backed UNMAP_QUEUE in a place whose
// failure can be seen and leaked deliberately, exactly like the one-shot path.
// A failed free leaks the ring and leaves the queue to the DRM file teardown.
void close_cached_session() {
  CachedSession& s = cached_session();
  if (!s.open) return;
  if (s.queue_open) {
    std::string detail;
    if (s.ring_open) ring_drained(s.ring.handle, s.slot, 250u, detail);
    if (loonggpu_userq_free(s.device, s.qid) != 0) {
      // The queue could not be confirmed gone.  Leave the ring mapped for the
      // DRM file teardown instead of unmapping a ring the firmware may still
      // own - that unmapped-then-recycled ring VA is the K16 fault shape.
      s.ring.leaked = true;
    }
    s.queue_open = false;
  }
  // EVERY cached BO handle is only valid while this DRM file is open.  Drop
  // them BEFORE closing: the move-assign never unmaps a resource (so anything
  // the firmware may still own stays mapped until the fd teardown reclaims
  // it), and clearing the state means a later submission opens a fresh
  // session, buffers and queue instead of reusing a handle from a closed file.
  s.ring = Bo{};
  s.text = Bo{};
  s.kd = Bo{};
  s.kernarg = Bo{};
  s.output = Bo{};
  s.signal = Bo{};
  s.tramp = Bo{};
  s.ring_open = false;
  s.text_open = false;
  s.kd_open = false;
  s.kernarg_open = false;
  s.output_open = false;
  s.signal_open = false;
  s.tramp_open = false;
  s.slot = 0;
  close_session(s.fd, s.device, s.ctx);
  s.fd = -1;
  s.device = nullptr;
  s.ctx = nullptr;
  s.open = false;
}

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
      0x100000100ull, 0x100000456ull, 0x100000a00ull,
      0, 0, 0x00004020ull);  // SGPR size 32 (the GS program uses s24), VGPR size 64
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

  CsLaunch l;
  l.group[0] = group_x;
  l.group[1] = group_y;
  l.group[2] = group_z;
  l.grid[0] = grid_x;
  l.grid[1] = grid_y;
  l.grid[2] = grid_z;

  loongvk_mcs_dispatch d;
  make_cs_dispatch(d, l);

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
  report.num_sgprs = d.num_sgprs;
  report.num_vgprs = d.num_vgprs;
  report.ok = true;
  return report;
}

// Build the .kd and the MCS stream for one launch, on the host.  Both the
// host-side dump and the real submit go through here, so they cannot drift.
bool cs_plan_bytes(const CsLaunch& l, std::vector<std::uint8_t>& kd,
                   std::vector<std::uint8_t>& stream, std::string& error) {
  loongvk_mcs_dispatch d;
  make_cs_dispatch(d, l);
  kd.resize(LOONGVK_KD_SIZE);
  if (!loongvk_mcs_patch_kd(kd.data(), kd.size(), &d)) {
    error = "loongvk_mcs_patch_kd failed (zero group dimension)";
    return false;
  }
  const std::uint32_t kd_flags_le = LOONGVK_KD_FLAGS;
  std::memcpy(kd.data() + 0, &kd_flags_le, sizeof kd_flags_le);
  std::vector<std::uint8_t> out(LOONGVK_MCS_MAX_LEN);
  std::size_t len = 0;
  if (!loongvk_mcs_build_dispatch(out.data(), out.size(), &d, &len)) {
    error = "loongvk_mcs_build_dispatch failed";
    return false;
  }
  stream.assign(out.begin(), out.begin() + static_cast<std::ptrdiff_t>(len));
  return true;
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
    if (request.group[i] > 0xffffu) {
      report.detail = "group dimension above the 16-bit launch field";
      return report;
    }
  }
  // The kernarg block is 8 (output pointer) + whatever the caller staged, and
  // must hold at least the 64-byte implicit-argument block the producer's
  // kernels read.  A short block is silent corruption, not an error.
  if (8u + request.kernarg_tail.size() < 8u) {
    report.detail = "kernarg block size overflow";
    return report;
  }

  CsLaunch launch;
  for (int i = 0; i < 3; ++i) {
    launch.group[i] = request.group[i];
    launch.grid[i] = request.grid[i];
  }
  launch.num_sgprs = request.num_sgprs;
  launch.num_vgprs = request.num_vgprs;
  launch.shared_bytes = request.shared_segment_size;

  int fd = -1;
  loonggpu_device_handle device = nullptr;
  loonggpu_context_handle ctx = nullptr;
  const bool persist = request.persistent_session;
  if (!acquire_session(persist, fd, device, ctx, report.detail)) {
    return report;
  }

  // Every buffer is a fixed-address BO, so their destructors unmap and free
  // them in reverse order of declaration.  The ring outlives the queue by
  // construction: the queue is freed explicitly below, before this scope ends.
  Bo ring_storage, text_storage, kd_storage, kernarg_storage, output_storage,
      signal_storage, tramp_storage;
  Bo* ring_ptr = nullptr;  // all set when the buffers are bound below
  Bo* text_ptr = nullptr;
  Bo* kd_ptr = nullptr;
  Bo* kernarg_ptr = nullptr;
  Bo* output_ptr = nullptr;
  Bo* signal_ptr = nullptr;
  Bo* tramp_ptr = nullptr;
  CachedSession* sess = persist ? &cached_session() : nullptr;
  const std::uint64_t ring_total =
      ((64u + kCsRingSize + kPageSize - 1) / kPageSize) * kPageSize;

  std::uint32_t qid = 0;
  bool queue_created = false;
  // A failed teardown must not release the ring: see Bo::leaked.
  auto abandon = [&](const char* what) {
    report.detail = what;
    if (queue_created) {
      const int rc = loonggpu_userq_free(device, qid);
      if (rc != 0) {
        if (ring_ptr) ring_ptr->leaked = true;
        if (text_ptr) text_ptr->leaked = true;
        if (kd_ptr) kd_ptr->leaked = true;
        if (kernarg_ptr) kernarg_ptr->leaked = true;
        if (output_ptr) output_ptr->leaked = true;
        if (signal_ptr) signal_ptr->leaked = true;
        report.detail +=
            " (queue teardown failed; the carrier's BOs are left mapped for "
            "the DRM file teardown, because releasing a ring the firmware may "
            "still own would let CPIPE dereference a recycled address)";
      }
      queue_created = false;
    } else if (persist && sess != nullptr && sess->queue_open) {
      // A persistent submission failed.  Retire the session's queue rather
      // than let the next submission publish into a queue the firmware may
      // already have dropped.  A free that fails leaks the ring and marks the
      // queue unusable, which is the same deliberate-leak rule as the one-shot
      // path: releasing a ring CPIPE may still own is how the VA gets recycled
      // under a live firmware queue slot.
      if (loonggpu_userq_free(device, qid) != 0) {
        sess->ring.leaked = true;
        report.detail += " (persistent queue teardown failed; the ring is left mapped)";
      }
      sess->queue_open = false;
    }
    return report;
  };
  auto fail = [&](const char* what) { return abandon(what); };

  // Page permissions.  The DRM VM path derives its PTEs from the BO's TTM
  // placement (loonggpu_ttm_tt_pte_flags), so these are intent rather than
  // mechanism - but the graphics carrier maps its program BOs readable +
  // executable and its data BOs readable + writable, and a compute kernel is
  // fetched and written through exactly the same MMU.  Passing 0 asks for a
  // mapping with no stated permissions, which is not a property this path has
  // ever been measured to tolerate.
  const std::uint64_t rw_flags =
      LOONGGPU_VM_PAGE_READABLE | LOONGGPU_VM_PAGE_WRITEABLE;
  const std::uint64_t rx_flags =
      LOONGGPU_VM_PAGE_READABLE | LOONGGPU_VM_PAGE_EXECUTABLE;

  // ---- the dispatch's buffers -------------------------------------------
  //
  // EVERY per-dispatch BO is cached with the queue when the caller asks for a
  // persistent session.  Creating and destroying them per submission is what
  // faults: CPIPE can still reference a fixed VA after the frontend has dropped
  // its mapping, and the two measured faults are exactly that - the ring VA
  // 0x100020000 first, then the text-region VA 0x100084000 once the ring
  // stopped moving.  One stable mapping for the life of the session removes the
  // whole class.  A one-shot submission (persist == false) keeps its own
  // buffers and full isolation.
  constexpr std::uint64_t kCsTextCacheBytes = 0x10000;    // 64 KiB of code
  constexpr std::uint64_t kCsOutputCacheBytes = 0x10000;  // 64 KiB of results
  if (request.text.size() > kCsTextCacheBytes ||
      request.output_size > kCsOutputCacheBytes) {
    release_session(persist, fd, device, ctx);
    report.detail = "program or result window exceeds the persistent cache size";
    return report;
  }

  // Bind one BO: a private allocation for a one-shot submission, or the
  // session's buffer allocated once and reused.  A later submission that needs
  // more than the cache was sized for is rejected above rather than silently
  // re-mapping a VA the firmware may be using.
  auto bind = [&](Bo& local, Bo* cached, bool* opened, std::uint64_t va,
                  std::uint64_t bytes, std::uint64_t flags,
                  std::uint64_t cache_bytes) -> Bo* {
    if (!persist) {
      if (!alloc_bo_at(device, local, va, bytes, flags, 0, report.detail))
        return nullptr;
      return &local;
    }
    if (!*opened) {
      if (!alloc_bo_at(device, *cached, va, std::max(bytes, cache_bytes), flags,
                       0, report.detail))
        return nullptr;
      *opened = true;
    }
    return cached;
  };

  // The kernarg needs room for the eight-byte output pointer plus whatever the
  // caller supplied, and never less than the implicit-argument block.  One
  // page: the same window budget the graphics argument BO has, because carrier
  // preloads address their own windows by offset (the resource tuples at
  // 0x200/0x230, the load window at 0x300, the texel window at 0x400).
  const std::uint64_t kernarg_bytes =
      std::max<std::uint64_t>(8u + request.kernarg_tail.size(), kCsKernargBytes);

  ring_ptr = bind(ring_storage, persist ? &sess->ring : nullptr,
                  persist ? &sess->ring_open : nullptr, kCsRingBaseVa, ring_total,
                  rw_flags, ring_total);
  tramp_ptr = bind(tramp_storage, persist ? &sess->tramp : nullptr,
                   persist ? &sess->tramp_open : nullptr,
                   carrier::kCsTrampolineVa, sizeof kTrampolineWords, rx_flags,
                   sizeof kTrampolineWords);
  text_ptr = bind(text_storage, persist ? &sess->text : nullptr,
                  persist ? &sess->text_open : nullptr, kCsTextVa,
                  request.text.size(), rx_flags, kCsTextCacheBytes);
  kd_ptr = bind(kd_storage, persist ? &sess->kd : nullptr,
                persist ? &sess->kd_open : nullptr, kCsKdVa, LOONGVK_KD_SIZE,
                rw_flags, LOONGVK_KD_SIZE);
  kernarg_ptr = bind(kernarg_storage, persist ? &sess->kernarg : nullptr,
                     persist ? &sess->kernarg_open : nullptr, kCsKernargVa,
                     kernarg_bytes, rw_flags, kCsKernargBytes);
  output_ptr = bind(output_storage, persist ? &sess->output : nullptr,
                    persist ? &sess->output_open : nullptr, kCsOutputVa,
                    request.output_size, rw_flags, kCsOutputCacheBytes);
  signal_ptr = bind(signal_storage, persist ? &sess->signal : nullptr,
                    persist ? &sess->signal_open : nullptr, kCsSignalVa,
                    kCsSignalBytes, rw_flags, kCsSignalBytes);
  if (ring_ptr == nullptr || tramp_ptr == nullptr || text_ptr == nullptr ||
      kd_ptr == nullptr || kernarg_ptr == nullptr || output_ptr == nullptr ||
      signal_ptr == nullptr) {
    release_session(persist, fd, device, ctx);
    return report;
  }
  Bo& ring = *ring_ptr;
  Bo& tramp = *tramp_ptr;
  Bo& text = *text_ptr;
  Bo& kd = *kd_ptr;
  Bo& kernarg = *kernarg_ptr;
  Bo& output = *output_ptr;
  Bo& signal = *signal_ptr;

  // Executable stub for an indirect control transfer (jirl).  See
  // carrier::kTrampolineVa: without it the jump has no admissible target.
  if (!poke_bo(tramp.handle, 0, kTrampolineWords, sizeof kTrampolineWords,
               report.detail)) {
    release_session(persist, fd, device, ctx);
    return fail("trampoline BO setup failed");
  }

  // Order matters: code, then descriptor, then kernarg and the input window,
  // then the signal; all of it before the queue exists, so the firmware can
  // never observe a half-written dispatch.
  if (!poke_bo(text.handle, 0, request.text.data(), request.text.size(),
               report.detail)) {
    release_session(persist, fd, device, ctx);
    return report;
  }

  std::vector<std::uint8_t> kd_bytes;
  std::vector<std::uint8_t> stream;
  std::string plan_error;
  if (!cs_plan_bytes(launch, kd_bytes, stream, plan_error)) {
    return fail(plan_error.c_str());
  }
  if (!poke_bo(kd.handle, 0, kd_bytes.data(), kd_bytes.size(), report.detail)) {
    release_session(persist, fd, device, ctx);
    return report;
  }

  // Kernarg: the output pointer first, then the caller's region.  This is the
  // layout the producer's kernels expect - their first instruction loads
  // exactly these eight bytes through s[6:7] - and the implicit-argument block
  // that follows is what get_num_groups()/get_local_size() read.
  {
    std::vector<std::uint8_t> ka(kernarg_bytes, 0);
    const std::uint64_t out_va = kCsOutputVa;
    std::memcpy(ka.data(), &out_va, sizeof out_va);
    if (!request.kernarg_tail.empty()) {
      std::memcpy(ka.data() + 8, request.kernarg_tail.data(),
                  request.kernarg_tail.size());
    }
    if (!poke_bo(kernarg.handle, 0, ka.data(), ka.size(), report.detail)) {
      release_session(persist, fd, device, ctx);
      return report;
    }
    report.wedge_kernarg = ka;
  }

  // A reused output BO must not carry the previous submission's results into
  // this one: a program that does not write every offset the runner reads would
  // then be judged against stale bytes.  A fresh BO was zeroed by the
  // allocator; a session-cached one has to be cleared explicitly.
  if (persist) {
    std::vector<std::uint8_t> zero(request.output_size, 0u);
    if (!poke_bo(output.handle, 0, zero.data(), zero.size(),
                 report.detail)) {
      release_session(persist, fd, device, ctx);
      return fail("output BO clear failed");
    }
  }

  // The carrier's input window, in the output BO the kernel stores into.  A
  // memory carrier stages its reads here.
  for (const auto& preload : request.output_preload) {
    if (!poke_bo(output.handle, preload.first, &preload.second,
                 sizeof preload.second, report.detail)) {
      release_session(persist, fd, device, ctx);
      return report;
    }
  }

  // Arm the completion signal with ONE packet before the doorbell.  (The
  // signal is a separate BO the firmware writes, so arming it here - before
  // the queue exists - cannot be observed by the firmware as a stale value.)  The device
  // DECREMENTS it once per retiring packet, so the wait is for zero; arming
  // with 0 and observing 0xffffffffffffffff is the same event.  A stale or
  // non-zero value here would make the wait below return immediately, so it is
  // written rather than assumed clear.
  {
    const std::uint64_t armed = 1;
    if (!poke_bo(signal.handle, 0, &armed, sizeof armed, report.detail)) {
      release_session(persist, fd, device, ctx);
      return report;
    }
  }

  // Create the queue ONCE per session, the way LoongVK does (one queue per
  // logical device).  The kernel refuses a second concurrent queue on the same
  // PASID anyway - loonggpu_drm_userq.c CREATE: "the LG200 userq teardown
  // packet is filtered by PASID, not by queue id" - so a per-submission
  // create/destroy is both the slow path and the racy one.  The ring starts
  // EMPTY (the allocate step zeroed it), so the firmware can never see a
  // stream that has not been published yet.
  const std::uint64_t queue_va = kCsRingBaseVa + 64u;
  if (persist) {
    if (!sess->queue_open) {
      if (loonggpu_userq_create(device, LOONGGPU_HW_IP_COMPUTE, queue_va,
                                kCsRingSize, &sess->qid) != 0) {
        release_session(persist, fd, device, ctx);
        return fail("loonggpu_userq_create(COMPUTE) failed");
      }
      sess->queue_open = true;
    }
    qid = sess->qid;
  } else if (loonggpu_userq_create(device, LOONGGPU_HW_IP_COMPUTE, queue_va,
                                   kCsRingSize, &qid) != 0) {
    release_session(persist, fd, device, ctx);
    return fail("loonggpu_userq_create(COMPUTE) failed");
  } else {
    queue_created = true;
  }

  // Ring placement and the write pointer, the reference driver's contract
  // (loongvk_compute.c / loongvk_mcs.h):
  //   wr_ptr   = queue_va - 64, i.e. the store at ring byte 0
  //   new_wptr = slot + mcs_len  (the END of the 64-byte-padded stream)
  //   slot     advances by the padded length and wraps when the next stream
  //            would not fit.
  // A fresh queue always starts at slot 0; a persistent one continues where
  // the previous stream ended.  The wptr must stay 64-aligned (the kernel's
  // DOORBELL rejects anything else) and strictly below the ring size.
  const std::size_t padded = (stream.size() + 63u) & ~std::size_t(63u);
  std::uint32_t slot = 0;
  if (persist) {
    slot = sess->slot;
    // A write pointer that goes BACKWARDS is refused by the firmware (measured:
    // wptr 384 with rptr 65472 -> "userq_doorbell failed (refused, not
    // dispatched)") and a wrong-offset parse has faulted instruction fetch at
    // 0x100084000.  So the ring is never wrapped in place.  When the next
    // stream would not fit, the QUEUE is recycled instead: drain, fence-backed
    // free, zero the index block, create a fresh queue whose MQD starts at rptr
    // 0, and begin again at slot 0.  That turns a create/destroy per submission
    // into one per ~340 streams while keeping every published pointer forward.
    if (static_cast<std::uint64_t>(slot) + padded + 64u > kCsRingSize) {
      std::string drain_detail;
      ring_drained(ring.handle, slot, 250u, drain_detail);
      const int rc = loonggpu_userq_free(device, qid);
      sess->queue_open = false;
      if (rc != 0) {
        sess->ring.leaked = true;
        report.status = ComputeSubmitReport::Status::Fault;
        report.detail =
            "ring full and the queue could not be recycled; the ring is left "
            "mapped and the session must not be reused";
        release_session(persist, fd, device, ctx);
        return report;
      }
      const std::uint64_t zero_pair[2] = {0, 0};
      if (!poke_bo(ring.handle, 0, zero_pair, sizeof zero_pair,
                   report.detail)) {
        release_session(persist, fd, device, ctx);
        return fail("index block reset failed");
      }
      if (loonggpu_userq_create(device, LOONGGPU_HW_IP_COMPUTE,
                                kCsRingBaseVa + 64u, kCsRingSize, &qid) != 0) {
        release_session(persist, fd, device, ctx);
        return fail("loonggpu_userq_create(COMPUTE) failed on recycle");
      }
      sess->qid = qid;
      sess->queue_open = true;
      slot = 0;
    }
  }
  std::vector<std::uint8_t> staged = stream;
  staged.resize(padded, 0u);
  if (!poke_bo(ring.handle, 64u + slot, staged.data(), staged.size(),
               report.detail)) {
    release_session(persist, fd, device, ctx);
    return fail("stream write failed");
  }
  const std::uint64_t wptr = slot + padded;
  if (!poke_bo(ring.handle, 0, &wptr, sizeof wptr, report.detail)) {
    release_session(persist, fd, device, ctx);
    return fail("write-index publish failed");
  }

  // Everything above is now the post-mortem record.
  report.wedge_stream = stream;
  report.wedge_kd = kd_bytes;
  {
    char fields[512];
    std::snprintf(
        fields, sizeof fields,
        "ring=0x%llx queue_va=0x%llx text=0x%llx kd=0x%llx kernarg=0x%llx "
        "output=0x%llx signal=0x%llx wptr=%llu stream=%zu kd_flags=0x%08x "
        "group=%u,%u,%u grid=%u,%u,%u sgprs=%u vgprs=%u lds=%u",
        static_cast<unsigned long long>(kCsRingBaseVa),
        static_cast<unsigned long long>(kCsRingBaseVa + 64u),
        static_cast<unsigned long long>(kCsTextVa),
        static_cast<unsigned long long>(kCsKdVa),
        static_cast<unsigned long long>(kCsKernargVa),
        static_cast<unsigned long long>(kCsOutputVa),
        static_cast<unsigned long long>(kCsSignalVa),
        static_cast<unsigned long long>(wptr), stream.size(),
        LOONGVK_KD_FLAGS, launch.group[0], launch.group[1], launch.group[2],
        launch.grid[0], launch.grid[1], launch.grid[2], launch.num_sgprs,
        launch.num_vgprs, launch.shared_bytes);
    report.wedge_fields = fields;
  }

  if (loonggpu_userq_doorbell(device, qid, wptr) != 0) {
    release_session(persist, fd, device, ctx);
    return fail("loonggpu_userq_doorbell failed (refused, not dispatched)");
  }

  // Poll the completion signal.  Map once and read through the mapping; the
  // device writes it, so every read has to come from memory rather than a
  // register copy.  The wait is for a CHANGE, not for a particular value: the
  // ABI measured on this driver is "arm 1, decrement to 0", but a value that
  // moved is completion either way, and which one it was is worth reporting.
  {
    void* sig_map = nullptr;
    if (loonggpu_bo_cpu_map(signal.handle, &sig_map) || !sig_map) {
      release_session(persist, fd, device, ctx);
      return fail("signal BO map failed");
    }
    const volatile std::uint64_t* sig =
        static_cast<const volatile std::uint64_t*>(sig_map);
    const auto deadline = std::chrono::steady_clock::now() +
                          std::chrono::milliseconds(request.timeout_ms);
    std::uint64_t seen = 1;
    for (;;) {
      std::this_thread::sleep_for(std::chrono::microseconds(200));
      seen = *sig;
      if (seen != 1) {
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
    if (seen == 1) {
      // The doorbell was accepted but nothing retired.  Treat the device as
      // wedged: do not release the ring, because the firmware may still be
      // translating it when the next mapping lands.
      release_session(persist, fd, device, ctx);
      return abandon("completion signal never moved (timeout)");
    }
  }

  // Read the output back.
  {
    void* out_map = nullptr;
    if (loonggpu_bo_cpu_map(output.handle, &out_map) || !out_map) {
      release_session(persist, fd, device, ctx);
      return fail("output BO map failed");
    }
    report.output.assign(static_cast<const std::uint8_t*>(out_map),
                         static_cast<const std::uint8_t*>(out_map) +
                             request.output_size);
    loonggpu_bo_cpu_unmap(output.handle);
  }

  // Do not free the queue while the command processor may still be fetching
  // from it.  A timeout here is not fatal on its own - the FREE below is
  // fence-backed - but it is reported, because a queue torn down before its
  // stream is consumed is exactly how a firmware slot is left pointing at an
  // address space the file is about to drop.
  {
    std::string drain_detail;
    if (!ring_drained(ring.handle, wptr, 250u, drain_detail))
      std::fprintf(stderr, "compute: ring not drained before teardown: %s\n",
                   drain_detail.c_str());
  }

  // Clean completion: the stream has been consumed.  A persistent session now
  // advances the ring slot and KEEPS the queue for the next submission - this
  // is the fix for the create/destroy churn - while a one-shot submission frees
  // the queue it created.  A FREE that fails still goes through the leak path,
  // because the kernel's FREE is fence-backed and a failure means the queue may
  // be live.
  if (persist) {
    // Advance past the 64-byte-aligned span this stream occupied.  Not
    // wrapped here: the next submission recycles the queue when the ring is
    // full, so every published write pointer stays forward.
    sess->slot = cs_ring_next_offset(slot, padded);
  } else {
    const int rc = loonggpu_userq_free(device, qid);
    queue_created = false;
    if (rc != 0) {
      ring.leaked = true;
      text.leaked = true;
      kd.leaked = true;
      kernarg.leaked = true;
      output.leaked = true;
      signal.leaked = true;
      report.status = ComputeSubmitReport::Status::Fault;
      report.detail =
          "the dispatch completed but the queue teardown failed; the ring is "
          "left mapped for the DRM file teardown";
      release_session(persist, fd, device, ctx);
      return report;
    }
  }

  // Clean means the doorbell was accepted, the signal moved, and the mapping
  // read back.  The signal's value is reported, not judged: 0 (armed 1,
  // decremented) and 0xffffffffffffffff (armed 0, underflowed) are both
  // lawful, and a value of 2 would say the device increments instead.
  report.status = ComputeSubmitReport::Status::Clean;
  release_session(persist, fd, device, ctx);
  return report;
}

const ComputeVaLayout& compute_va_layout() {
  static const ComputeVaLayout layout{kCsRingBaseVa, kCsTextVa, kCsKdVa,
                                      kCsKernargVa, kCsOutputVa, kCsSignalVa};
  return layout;
}

// The vendor's own 192-byte stream for the live 1x1x1 add.cl dispatch, captured
// at AqlQueue::SubmitMCS and padded to the 64-byte alignment this driver
// publishes.  Copied from the reference the LoongVK unit test pins
// (src/mesa/src/loonggpu/vulkan/tests/test_loongvk_mcs.c), which in turn came
// from analysis/lacm/lggpu-abi/MCS-ENCODING.md; the values are the capture's,
// not chosen here.
constexpr std::uint8_t kVendorRef1x1x1[192] = {
    0x89, 0x01, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x89, 0x08, 0x40, 0x00, 0xa8, 0x99, 0x00, 0xe8,
    0x13, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x76, 0x20, 0x3c, 0x08,
    0x89, 0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x00, 0x00, 0x00, 0x00, 0xac, 0x4f, 0xca, 0x00, 0x89, 0x14, 0x20, 0x00,
    0x20, 0x01, 0x20, 0xe8, 0x13, 0x00, 0x00, 0x00, 0x89, 0x16, 0x20, 0x00,
    0x80, 0x00, 0x20, 0xe8, 0x13, 0x00, 0x00, 0x00, 0x89, 0x18, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x1a, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x87, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x8d, 0x00, 0x00, 0x00,
    0x8b, 0x0f, 0x00, 0x00, 0x8a, 0x00, 0x00, 0x00, 0x80, 0xf7, 0xbf, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// The same vendor emitters driven with the CTS geometry (2x2x4 workgroups of
// 1x4x2 invocations).  Only the aperture workgroup word and the three group
// counts differ from the 1x1x1 reference; checking both matters because a
// transposed or swapped dimension is invisible in the 1x1x1 bytes.
constexpr std::uint8_t kVendorRefCts[192] = {
    0x89, 0x01, 0x30, 0x00, 0x00, 0x0c, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x89, 0x08, 0x40, 0x00, 0xa8, 0x99, 0x00, 0xe8,
    0x13, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x76, 0x20, 0x3c, 0x08,
    0x89, 0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x00, 0x00, 0x00, 0x00, 0xac, 0x4f, 0xca, 0x00, 0x89, 0x14, 0x20, 0x00,
    0x20, 0x01, 0x20, 0xe8, 0x13, 0x00, 0x00, 0x00, 0x89, 0x16, 0x20, 0x00,
    0x80, 0x00, 0x20, 0xe8, 0x13, 0x00, 0x00, 0x00, 0x89, 0x18, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x1a, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x87, 0x07, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x8d, 0x00, 0x00, 0x00,
    0x8b, 0x0f, 0x00, 0x00, 0x8a, 0x00, 0x00, 0x00, 0x80, 0xf7, 0xbf, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

bool stream_matches(const std::vector<std::uint8_t>& got,
                    const std::uint8_t* want, std::size_t want_size,
                    const char* label, std::string& detail) {
  if (got.size() != want_size) {
    detail = std::string(label) + ": length " + std::to_string(got.size()) +
             ", vendor reference is " + std::to_string(want_size);
    return false;
  }
  for (std::size_t i = 0; i < want_size; ++i) {
    if (got[i] != want[i]) {
      char line[160];
      std::snprintf(line, sizeof line,
                    "%s: first difference at +0x%zx: built 0x%02x, vendor "
                    "0x%02x",
                    label, i, got[i], want[i]);
      detail = line;
      return false;
    }
  }
  return true;
}

// Put the captured 64-bit addresses into the built stream, so the byte compare
// is against the capture rather than against this carrier's own layout.  The
// command order, ids, payload lengths and every shape word are still the
// builder's, which is what the check exists to pin.
void substitute_ref_addresses(std::vector<std::uint8_t>& s, std::uint64_t kd_va,
                              std::uint64_t entry, std::uint64_t kernarg,
                              std::uint64_t signal) {
  const auto put = [&s](std::size_t off, std::uint64_t v) {
    for (int i = 0; i < 4; ++i) {
      s[off + i] = static_cast<std::uint8_t>(v >> (8 * i));
      s[off + 4 + i] = static_cast<std::uint8_t>(v >> (32 + 8 * i));
    }
  };
  // Offsets are the vendor stream's payload positions, read off the reference:
  //   +0x14  the id-0x20 block's entry dwords
  //   +0x3c  the id-0x50 block's kernel_object dwords
  //   +0x48  the id-0x58 block's kernarg dwords
  //   +0x80  the sbmt command's signal dwords
  put(0x14, entry);
  put(0x3c, kd_va);
  put(0x48, kernarg);
  put(0x80, signal);
}

// The 2026-08-24 vendor capture was dispatched with FLUSH_NONE (rsrc1
// 0x083C2076).  This carrier requests FLUSH_SOURCE_OUTPUT instead, so the only
// permitted difference from the capture is the four float-mode fields at stream
// +0x20.  Rewrite exactly those bits and require every other byte to match.
std::vector<std::uint8_t> with_carrier_float_mode(const std::uint8_t* ref,
                                                  std::size_t n) {
  std::vector<std::uint8_t> out(ref, ref + n);
  std::uint32_t rsrc1 = 0;
  std::memcpy(&rsrc1, out.data() + 0x20, sizeof rsrc1);
  constexpr std::uint32_t kModeMask = 0xffu << 14;  // round/denorm, bits 14:21
  rsrc1 = (rsrc1 & ~kModeMask) | kFloatModeFlush;
  std::memcpy(out.data() + 0x20, &rsrc1, sizeof rsrc1);
  return out;
}

bool compute_selfcheck(std::string& detail) {
  constexpr std::uint64_t kRefKd = 0x00000013e8200120ull;
  constexpr std::uint64_t kRefEntry = 0x00000013e80099a8ull;
  constexpr std::uint64_t kRefKernarg = 0x00000013e8200080ull;
  constexpr std::uint64_t kRefSignal = 0x0000000400bff780ull;

  // The captured dispatch's own values: every field was read from the vendor
  // runtime at a breakpoint, not chosen here.  num_sgprs/num_vgprs reproduce
  // the .kd +0x30 word 0x42 (two VGPR granules, one SGPR granule).
  CsLaunch live;
  live.group[0] = live.group[1] = live.group[2] = 1;
  live.grid[0] = live.grid[1] = live.grid[2] = 1;
  live.num_sgprs = 8;
  live.num_vgprs = 8;

  std::vector<std::uint8_t> kd;
  std::vector<std::uint8_t> stream;
  std::string error;
  if (!cs_plan_bytes(live, kd, stream, error)) {
    detail = "compute_selfcheck: " + error;
    return false;
  }
  substitute_ref_addresses(stream, kRefKd, kRefEntry, kRefKernarg, kRefSignal);
  const std::vector<std::uint8_t> ref1x1x1 =
      with_carrier_float_mode(kVendorRef1x1x1, sizeof kVendorRef1x1x1);
  if (!stream_matches(stream, ref1x1x1.data(), ref1x1x1.size(),
                      "1x1x1 stream", detail)) {
    return false;
  }

  // CTS geometry.  The dispatch supplies a workgroup COUNT while the AQL
  // grid_size the stream and the .kd carry is the GLOBAL work-item count, so it
  // is multiplied out here; passing the count through would divide by the local
  // size a second time.
  CsLaunch cts;
  cts.group[0] = 1; cts.group[1] = 4; cts.group[2] = 2;
  cts.grid[0] = 2 * 1; cts.grid[1] = 2 * 4; cts.grid[2] = 4 * 2;
  cts.num_sgprs = 8;
  cts.num_vgprs = 8;
  std::vector<std::uint8_t> cts_kd;
  std::vector<std::uint8_t> cts_stream;
  if (!cs_plan_bytes(cts, cts_kd, cts_stream, error)) {
    detail = "compute_selfcheck: " + error;
    return false;
  }
  substitute_ref_addresses(cts_stream, kRefKd, kRefEntry, kRefKernarg,
                           kRefSignal);
  const std::vector<std::uint8_t> ref_cts =
      with_carrier_float_mode(kVendorRefCts, sizeof kVendorRefCts);
  if (!stream_matches(cts_stream, ref_cts.data(), ref_cts.size(),
                      "CTS-geometry stream", detail)) {
    return false;
  }
  // The 1x1x1 reference is blind to the local-size packing (every field is 0
  // there), so pin the packed word explicitly: local 1x4x2 is
  // (0) | (3 << 10) | (1 << 20) = 0x00100c00.
  if (cts_stream[4] != 0x00 || cts_stream[5] != 0x0c || cts_stream[6] != 0x10 ||
      cts_stream[7] != 0x00) {
    detail = "compute_selfcheck: the aperture workgroup word is not the "
             "vendor's 0x00100c00 for local 1x4x2";
    return false;
  }

  // The kernel descriptor's resource words must equal the stream's own copy of
  // them, or the descriptor and the stream describe different launches.
  for (std::size_t i = 0; i < 4; ++i) {
    if (kd[48 + i] != stream[0x1c + i] || kd[52 + i] != stream[0x20 + i]) {
      detail =
          "compute_selfcheck: the .kd resource words differ from the stream's "
          "id-0x20 copy";
      return false;
    }
  }

  char line[320];
  std::snprintf(line, sizeof line,
                "MCS stream is byte-identical to the vendor's own bytes for "
                "both the live 1x1x1 add.cl dispatch and the CTS "
                "2x2x4-groups/1x4x2-local geometry (%zu bytes each); .kd "
                "flags=0x%08x entry=0x%08x rsrc0=0x%08x rsrc1=0x%08x",
                stream.size(), 0x0facu,
                static_cast<unsigned>(kd[32]) |
                    (static_cast<unsigned>(kd[33]) << 8) |
                    (static_cast<unsigned>(kd[34]) << 16) |
                    (static_cast<unsigned>(kd[35]) << 24),
                static_cast<unsigned>(kd[48]) |
                    (static_cast<unsigned>(kd[49]) << 8) |
                    (static_cast<unsigned>(kd[50]) << 16) |
                    (static_cast<unsigned>(kd[51]) << 24),
                static_cast<unsigned>(kd[52]) |
                    (static_cast<unsigned>(kd[53]) << 8) |
                    (static_cast<unsigned>(kd[54]) << 16) |
                    (static_cast<unsigned>(kd[55]) << 24));
  detail = line;
  return true;
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

const ComputeVaLayout& compute_va_layout() {
  static const ComputeVaLayout layout{0x100020000ull, 0x100080000ull,
                                      0x100090000ull, 0x1000a0000ull,
                                      0x1000b0000ull, 0x1000c0000ull};
  return layout;
}

bool compute_selfcheck(std::string& detail) {
  detail = "no libdrm_loonggpu; the stream builder is still host-side, so this "
           "is not a DRM-dependent check (unreachable in this build)";
  return false;
}

#endif  // LGPU_HAVE_LIBDRM

}  // namespace lgpu::drm