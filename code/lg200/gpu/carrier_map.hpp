#pragma once

// Carrier-side fixed layout, one definition per fact.
//
// Argument-BO byte map (VA base = kArgVa, BO size 0x2000):
//   0x020  per-case result dwords (every carrier stores here)
//   0x200  rsd-table slot: image T# (8 dwords) at +0, sampler S# (4) at +0x30
//   0x300  buff/flat preload window
//   0x400  linear texel rows (texel(x,y) = base + y*pitch*4 + x*4)
//
// Scalar register windows:
//   s0-s3   result-store buffer descriptor {lo=0, hi=1, 0x40, 0x27fac};
//           the first pair doubles as the vector-flat base (= kArgVa)
//   s4-s7   hydrated by hardware at wave start (never clobber)
//   s8-s15  image T# tuple (scalar-flat loaded from the rsd table)
//   s16-s19 sampler S# tuple (scalar-flat loaded from rsd table + 0x30)
//   s20     scratch immediate staging in the samp family (writes SCC)
//
// Vector register windows:
//   v0      zero address for result stores (build_ps and the flat family keep
//           v9: build_ps stages vector sources at v0, flat's address pair is
//           v0:v1)
//   v8-v11  target result quad
//   v12-v15 post-op readback quad
//   v16+    coordinate staging (per-carrier width)

#include "codec/operand.hpp"

#include <cstdint>

namespace lgpu::carrier {

constexpr std::uint64_t kArgVa       = 0x100000000ull;
// Resource 1 of the PS-control packet: the colour / render-target BO.  The
// correctness carriers store into the argument BO (the host reads results from
// there); the bench carrier stores here instead so that its writes do not
// collide with the 0x13 timestamp pair, which lives in the argument BO.
constexpr std::uint64_t kColorVa     = 0x100004000ull;
constexpr std::uint32_t kResultBase  = 0x020;
constexpr std::uint32_t kRsdTable    = 0x200;
constexpr std::uint32_t kLoadWindow  = 0x300;
constexpr std::uint32_t kTexelWindow = 0x400;
constexpr std::uint32_t kSamplerSlot = 0x230;  // rsd-table + 0x30
// Bytes of the argument BO a carrier may address with a case-derived offset.
// A carrier that computes an address from case data must clamp to this: the
// argument BO is one page, and an access beyond it is a GPU protection fault
// that quarantines the pipe rather than returning an error (measured: one such
// case faulted at arg_va + 0x12345678 and needed a platform power cycle).
constexpr std::uint32_t kArgWindowBytes = 0x2000;

// A tiny executable stub: `nop` then `exit`, mapped at a fixed VA on both
// pipes.  An indirect control transfer (jirl) needs a target that is a real,
// admitted control-flow position; the vendor's own kernels reach one through
// its linker's relocation pass, and the carriers reach this one by constant.
// Without it a jirl's target is whatever the wave happens to hold, and the
// instruction fetch faults (measured at address 0x8, costing a power cycle).
constexpr std::uint64_t kTrampolineVa = 0x100040000ull;  // clear of the PS block (0x100000000..0x100018000)
                                                        // and of the stage programs (..0x100034000)
constexpr std::uint64_t kCsTrampolineVa = 0x1000d0000ull;    // after the CS signal

// PS-probe roots: the three hydrated descriptor bases submit_probe stages.
struct ProbeRoots {
  std::uint64_t const_buffer;
  std::uint64_t constant;
  std::uint64_t rsd;
};
inline constexpr ProbeRoots kProbeRoots{kArgVa + 0x600, kArgVa + 0x300,
                                        kArgVa + kRsdTable};

// Compute (CPIPE) carrier window.  The PS block above is
// 0x100000000..0x100018000; these BOs are mapped at fixed addresses rather than
// through libdrm's VA manager, so the compute window is placed clear of it by
// hand.  One BO holds the 64-byte user-queue index block followed by the ring,
// so the ring VA is kCsRingBaseVa + 64.
//
// These are the addresses the assembled compute programs carry AND the ones
// drm::submit_compute maps its BOs at, so they are defined once, here, and the
// layout is published through drm::compute_va_layout().  A program that stores
// to a different address than the BO is mapped at does not fail loudly: it
// wedges the pipe, which costs a power cycle.
constexpr std::uint64_t kCsRingBaseVa = 0x100020000ull;
constexpr std::uint64_t kCsTextVa     = 0x100080000ull;
constexpr std::uint64_t kCsKdVa       = 0x100090000ull;
constexpr std::uint64_t kCsKernargVa  = 0x1000a0000ull;
// The one buffer a compute program reads its staged inputs from and writes its
// results to.  submit_compute names it in the kernarg block's first user
// argument (so a compiled-style kernel reaches it through s[6:7]); this
// carrier's programs assemble it as a literal instead, which keeps the result
// path independent of the launch state.
constexpr std::uint64_t kCsArgVa      = 0x1000b0000ull;
constexpr std::uint64_t kCsSignalVa   = 0x1000c0000ull;

// Register windows (see file comment).
inline constexpr isa::Reg kStoreDesc()  { return isa::sgpr(0, 4); }
inline constexpr isa::Reg kFlatBase()   { return isa::sgpr(0, 2); }
inline constexpr isa::Reg kImageTuple() { return isa::sgpr(8, 8); }
inline constexpr isa::Reg kSamplerTuple() { return isa::sgpr(16, 4); }

}  // namespace lgpu::carrier
