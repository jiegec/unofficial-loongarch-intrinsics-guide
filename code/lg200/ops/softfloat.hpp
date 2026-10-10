#pragma once

#include <cstdint>

namespace lgpu::test {

// Explicit FP policy carried by every FP case; the reference never trusts
// the host FP mode (see roadmap oracle layering).
struct FpPolicy {
  enum class Rounding { NearestEven, TowardZero, TowardPositive, TowardNegative } rounding =
      Rounding::NearestEven;
  bool ftz = false;  // flush denormal outputs to zero
  bool daz = false;  // treat denormal inputs as zero
};

// Host-independent IEEE-754 binary32 operations, bit-exact. NaN payloads are
// canonicalized (quiet bit set, low payload preserved). add/sub/mul are
// correctly rounded for all four rounding modes. min/max follow IEEE 754-2008
// minNum/maxNum with the NaN behavior defined by the instruction cases.
std::uint32_t sf_add32(std::uint32_t a, std::uint32_t b, FpPolicy policy);
std::uint32_t sf_sub32(std::uint32_t a, std::uint32_t b, FpPolicy policy);
std::uint32_t sf_revsub32(std::uint32_t a, std::uint32_t b, FpPolicy policy);
std::uint32_t sf_mul32(std::uint32_t a, std::uint32_t b, FpPolicy policy);
std::uint32_t sf_min32(std::uint32_t a, std::uint32_t b, FpPolicy policy);
std::uint32_t sf_max3(std::uint32_t a, std::uint32_t b, std::uint32_t c, FpPolicy policy);
std::uint32_t sf_min3(std::uint32_t a, std::uint32_t b, std::uint32_t c, FpPolicy policy);
std::uint32_t sf_med3(std::uint32_t a, std::uint32_t b, std::uint32_t c, FpPolicy policy);
std::uint32_t sf_sqrt32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_max32(std::uint32_t a, std::uint32_t b, FpPolicy policy);
std::uint32_t sf_abs32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_neg32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_copysign32(std::uint32_t a, std::uint32_t b, FpPolicy policy);
std::uint32_t sf_fract32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_floor32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_ceil32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_trunc32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_rndne32(std::uint32_t a, FpPolicy policy);

// Conversions. i32/u32 -> f32 use RTNE; f32 -> i32/u32 truncate toward zero
// and clamp; f16 conversions round per
// policy (default RTZ for pack forms, RTNE for v_fcvt_f16_f32).
std::uint32_t sf_i32_f32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_u32_f32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_f32_i32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_f32_u32(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_f32_f16(std::uint32_t a, FpPolicy policy);
std::uint32_t sf_f16_f32(std::uint32_t a, FpPolicy policy);

// binary64 arithmetic (same explicit policy; 53-bit significands).
std::uint64_t sf_add64(std::uint64_t a, std::uint64_t b, FpPolicy policy);
std::uint64_t sf_sub64(std::uint64_t a, std::uint64_t b, FpPolicy policy);
std::uint64_t sf_mul64(std::uint64_t a, std::uint64_t b, FpPolicy policy);
std::uint64_t sf_min64(std::uint64_t a, std::uint64_t b);
std::uint64_t sf_max64(std::uint64_t a, std::uint64_t b);
std::uint64_t sf_fma64(std::uint64_t a, std::uint64_t b, std::uint64_t c,
                       FpPolicy policy);
std::uint64_t sf_ldexp64(std::uint64_t a, std::uint32_t exp);
std::uint64_t sf_sqrt64(std::uint64_t a, FpPolicy policy);
std::uint64_t sf_f32_f64(std::uint32_t a);
std::uint32_t sf_f64_f32(std::uint64_t a);
std::uint64_t sf_i32_f64(std::uint32_t a);
std::uint32_t sf_f64_i32(std::uint64_t a, FpPolicy policy);
std::uint64_t sf_u32_f64(std::uint32_t a);
std::uint32_t sf_f64_u32(std::uint64_t a, FpPolicy policy);

}  // namespace lgpu::test
