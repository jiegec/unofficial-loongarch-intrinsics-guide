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
constexpr std::uint32_t kResultBase  = 0x020;
constexpr std::uint32_t kRsdTable    = 0x200;
constexpr std::uint32_t kLoadWindow  = 0x300;
constexpr std::uint32_t kTexelWindow = 0x400;
constexpr std::uint32_t kSamplerSlot = 0x230;  // rsd-table + 0x30

// PS-probe roots: the three hydrated descriptor bases submit_probe stages.
struct ProbeRoots {
  std::uint64_t const_buffer;
  std::uint64_t constant;
  std::uint64_t rsd;
};
inline constexpr ProbeRoots kProbeRoots{kArgVa + 0x600, kArgVa + 0x300,
                                        kArgVa + kRsdTable};

// Register windows (see file comment).
inline constexpr isa::Reg kStoreDesc()  { return isa::sgpr(0, 4); }
inline constexpr isa::Reg kFlatBase()   { return isa::sgpr(0, 2); }
inline constexpr isa::Reg kImageTuple() { return isa::sgpr(8, 8); }
inline constexpr isa::Reg kSamplerTuple() { return isa::sgpr(16, 4); }

}  // namespace lgpu::carrier
