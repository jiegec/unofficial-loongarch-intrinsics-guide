// LG200 oracle authoring macros.
//
// One instruction = one file pair under code/lg200/:
//   <mnemonic>.h    executable pseudocode, included inside the oracle
//                   function body and rendered verbatim as the manual's
//                   Operation section (unofficial-loongarch-intrinsics-guide
//                   style)
//   <mnemonic>.cpp  the oracle function (LG200_ORACLE_BEGIN/END sandwich with
//                   #include "<mnemonic>.h") plus the deterministic case
//                   vectors (LG200_CASE_BEGIN/END with CASE rows)
//
// The pseudocode body may use:
//   src[n]   the nth input dword
//   dst[n]   the nth result dword (at most 8)
//   policy   the test::FpPolicy for floating-point rounding and flush modes
//   test::   shared helpers from ops/oracle.hpp (softfloat, compares, ...)
#pragma once

#include "ops/oracle.hpp"

#include <cstdint>
#include <vector>

// Short type names for pseudocode bodies, so the .h reads like pseudocode
// instead of C++ boilerplate (same convention as the
// unofficial-loongarch-intrinsics-guide common.h).
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;
using f32 = float;
using f64 = double;

#include "ops/pseudo.hpp"

#define LG200_ORACLE_BEGIN(name, nin)                                          \
  namespace lgpu::ops {                                                        \
  bool oracle_##name(const std::vector<std::uint32_t>& inputs,                 \
                     const test::FpPolicy& policy,                             \
                     std::vector<std::uint32_t>& results) {                    \
    static_assert((nin) <= 32, "LG200 oracle: too many input words");          \
    if (inputs.size() < (nin) || ((nin) == 0 && !inputs.empty())) {            \
      return false;                                                            \
    }                                                                          \
    const std::uint32_t* src = inputs.data();                                  \
    std::uint32_t dst[8] = {};                                                 \
    (void)src;                                                                 \
    (void)policy;

#define LG200_ORACLE_END(nout)                                                 \
    static_assert((nout) <= 8, "LG200 oracle: too many result words");         \
    results.assign(dst, dst + (nout));                                         \
    return true;                                                               \
  }                                                                            \
  }  // namespace lgpu::ops

namespace lgpu::test {
// Deterministic CPU vector: input dwords followed by expected dwords.
// fp is the floating-point policy token string ("" for integer cases).
struct Vector {
  const char* id;
  const char* fp;
  std::uint32_t words[32];
};
}  // namespace lgpu::test

#define LG200_CASE_BEGIN(name)                                                 \
  [[maybe_unused]] static constexpr lgpu::test::Vector lg200_cases_##name[] = {
#define CASE(id, ...) { #id, "", { __VA_ARGS__ } },
#define CASE_FP(id, fp, ...) { #id, #fp, { __VA_ARGS__ } },
#define LG200_CASE_END                                                         \
  };
