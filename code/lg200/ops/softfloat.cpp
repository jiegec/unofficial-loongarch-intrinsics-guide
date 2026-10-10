// Bit-exact IEEE-754 binary16/32/64 reference operations for the LG200 CPU
// oracles, backed by Berkeley SoftFloat Release 3e (vendored under
// ops/berkeley-softfloat-3/, BSD-3-Clause).  The correctly-rounded arithmetic
// core is delegated to the library; what remains here is policy plumbing
// (rounding mode, daz/ftz) and the instruction-specific bit rules (GCN-style
// min/max NaN behavior, integer-conversion clamps, ldexp).  The reference
// never trusts the host FP mode.
//
// NaN convention follows the library (x86 default): derived (invalid) NaNs are
// the canonical quiet NaN 0xFFC00000 / 0xFFF8000000000000, and propagated
// NaNs keep the source operand's sign and payload.

#include "softfloat.hpp"

#include <cstdint>

extern "C" {
#include "berkeley-softfloat-3/source/include/softfloat.h"
}

namespace lgpu::test {
namespace {

// ---- binary32 bit helpers ---------------------------------------------------
constexpr std::uint32_t kSign = 0x80000000u;
constexpr std::uint32_t kExpMask = 0x7f800000u;
constexpr std::uint32_t kMantMask = 0x007fffffu;
constexpr std::uint32_t kQnan = 0x7fc00000u;

struct Decomposed {
  bool sign;
  int exp;  // unbiased; -126 for subnormal, -127 for zero
  std::uint64_t mant;  // 24-bit significand (implicit bit for normals)
  bool is_nan;
  bool is_inf;
  bool is_zero;
};

Decomposed decompose(std::uint32_t bits) {
  Decomposed out{};
  out.sign = (bits & kSign) != 0;
  const std::uint32_t exp = (bits & kExpMask) >> 23;
  const std::uint32_t mant = bits & kMantMask;
  out.is_nan = exp == 0xffu && mant != 0;
  out.is_inf = exp == 0xffu && mant == 0;
  if (exp == 0) {
    out.is_zero = mant == 0;
    out.exp = out.is_zero ? -127 : -126;
    out.mant = mant;
  } else {
    out.exp = static_cast<int>(exp) - 127;
    out.mant = (1ull << 23) | mant;
  }
  return out;
}

std::uint32_t compose(bool sign, int exp, std::uint64_t mant) {
  return (sign ? kSign : 0) |
         (static_cast<std::uint32_t>(exp + 127) << 23) |
         (static_cast<std::uint32_t>(mant) & kMantMask);
}

std::uint32_t canonical_nan(std::uint32_t bits) {
  return (bits & kSign) | kQnan | (bits & kMantMask);
}

std::uint32_t minmax(std::uint32_t a, std::uint32_t b, bool want_min) {
  const Decomposed da = decompose(a);
  const Decomposed db = decompose(b);
  if (da.is_nan) {
    return db.is_nan ? canonical_nan(a) : a;
  }
  if (db.is_nan) {
    return b;
  }
  if (da.is_zero && db.is_zero) {
    if (want_min) {
      return (da.sign || db.sign) ? kSign : 0u;
    }
    return (da.sign && db.sign) ? kSign : 0u;
  }
  bool a_less;
  if (da.sign != db.sign) {
    a_less = da.sign;
  } else if (da.exp != db.exp) {
    a_less = (da.exp < db.exp) != da.sign;
  } else if (da.mant != db.mant) {
    a_less = (da.mant < db.mant) != da.sign;
  } else {
    a_less = false;
  }
  return (want_min ? a_less : !a_less) ? a : b;
}

std::uint32_t round_integer(std::uint32_t a, FpPolicy::Rounding mode) {
  const Decomposed d = decompose(a);
  if (d.is_nan) {
    return canonical_nan(a);
  }
  if (d.is_inf || d.is_zero) {
    return a;
  }
  if (d.exp < 0) {
    // |value| < 1 with a nonzero significand: always has a fraction. The
    // zero result keeps the operand sign.
    if (mode == FpPolicy::Rounding::TowardPositive) {
      return d.sign ? kSign : 0x3f800000u;
    }
    if (mode == FpPolicy::Rounding::TowardNegative) {
      return d.sign ? (kSign | 0x3f800000u) : 0u;
    }
    return d.sign ? kSign : 0;
  }
  const unsigned keep_bits = static_cast<unsigned>(d.exp) + 1;
  unsigned drop;
  if (keep_bits >= 24) {
    drop = 0;
  } else {
    drop = 24 - keep_bits;
  }
  const std::uint64_t low = drop ? (d.mant & ((1ull << drop) - 1)) : 0;
  const std::uint64_t kept = d.mant >> drop;
  const bool round_up = [&] {
    switch (mode) {
      case FpPolicy::Rounding::TowardZero:
        return false;
      case FpPolicy::Rounding::TowardPositive:
        return !d.sign && low != 0;
      case FpPolicy::Rounding::TowardNegative:
        return d.sign && low != 0;
      case FpPolicy::Rounding::NearestEven: {
        const std::uint64_t half = 1ull << (drop - 1);
        return low > half || (low == half && (kept & 1) != 0);
      }
    }
    return false;
  }();
  std::uint64_t value = round_up ? kept + 1 : kept;
  int extra_exp = 0;
  if (value >= (1ull << 24)) {
    value >>= 1;
    extra_exp = 1;
  }
  if (d.exp >= 23) {
    return compose(d.sign, d.exp + extra_exp, value);
  }
  const unsigned bits = 64 - __builtin_clzll(value);
  const std::uint64_t sig = value << (24 - bits);
  return compose(d.sign, static_cast<int>(bits) - 1 + extra_exp, sig);
}

// ---- binary64 bit helpers ----------------------------------------------------
constexpr std::uint64_t kS64 = 0x8000000000000000ull;
constexpr std::uint64_t kE64 = 0x7ff0000000000000ull;
constexpr std::uint64_t kM64 = 0x000fffffffffffffull;
constexpr std::uint64_t kQnan64 = 0x7ff8000000000000ull;

struct D64 {
  bool sign;
  int exp;  // unbiased; -1022 for subnormal, -1023 for zero
  std::uint64_t mant;  // 53-bit significand (implicit bit for normals)
  bool is_nan;
  bool is_inf;
  bool is_zero;
};

D64 decompose64(std::uint64_t bits) {
  D64 out{};
  out.sign = (bits & kS64) != 0;
  const std::uint64_t exp = (bits & kE64) >> 52;
  const std::uint64_t mant = bits & kM64;
  out.is_nan = exp == 0x7ff && mant != 0;
  out.is_inf = exp == 0x7ff && mant == 0;
  if (exp == 0) {
    out.is_zero = mant == 0;
    out.exp = out.is_zero ? -1023 : -1022;
    out.mant = mant;
  } else {
    out.exp = static_cast<int>(exp) - 1023;
    out.mant = (1ull << 52) | mant;
  }
  return out;
}

std::uint64_t compose64(bool sign, int exp, std::uint64_t mant) {
  return (sign ? kS64 : 0) |
         (static_cast<std::uint64_t>(exp + 1023) << 52) |
         (mant & kM64);
}

std::uint64_t canonical_nan64(std::uint64_t bits) {
  return (bits & kS64) | kQnan64 | (bits & kM64);
}

std::uint64_t minmax64(std::uint64_t a, std::uint64_t b, bool want_min) {
  const D64 da = decompose64(a);
  const D64 db = decompose64(b);
  if (da.is_nan) {
    return db.is_nan ? canonical_nan64(a) : a;
  }
  if (db.is_nan) {
    return b;
  }
  if (da.is_zero && db.is_zero) {
    if (want_min) {
      return (da.sign || db.sign) ? kS64 : 0;
    }
    return (da.sign && db.sign) ? kS64 : 0;
  }
  bool a_less;
  if (da.sign != db.sign) {
    a_less = da.sign;
  } else if (da.exp != db.exp) {
    a_less = (da.exp < db.exp) != da.sign;
  } else if (da.mant != db.mant) {
    a_less = (da.mant < db.mant) != da.sign;
  } else {
    a_less = false;
  }
  return (want_min ? a_less : !a_less) ? a : b;
}

// ---- Berkeley glue ------------------------------------------------------------
inline float32_t mk32(std::uint32_t b) { float32_t f; f.v = b; return f; }
inline std::uint32_t bk32(float32_t f) { return f.v; }
inline float64_t mk64(std::uint64_t b) { float64_t f; f.v = b; return f; }
inline std::uint64_t bk64(float64_t f) { return f.v; }
inline float16_t mk16(std::uint16_t b) { float16_t f; f.v = b; return f; }
inline std::uint16_t bk16(float16_t f) { return f.v; }

inline void set_round(FpPolicy p) {
  switch (p.rounding) {
    case FpPolicy::Rounding::NearestEven:
      softfloat_roundingMode = softfloat_round_near_even;
      break;
    case FpPolicy::Rounding::TowardZero:
      softfloat_roundingMode = softfloat_round_minMag;
      break;
    case FpPolicy::Rounding::TowardPositive:
      softfloat_roundingMode = softfloat_round_max;
      break;
    case FpPolicy::Rounding::TowardNegative:
      softfloat_roundingMode = softfloat_round_min;
      break;
  }
}

inline std::uint32_t flush32(std::uint32_t b) {
  if ((b & kExpMask) == 0 && (b & kMantMask)) return b & kSign;
  return b;
}
inline std::uint64_t flush64(std::uint64_t b) {
  if ((b & kE64) == 0 && (b & kM64)) return b & kS64;
  return b;
}
// LG200 hardware derives NaN with a positive sign (0x7FC00000);
// Berkeley's x86 default is 0xFFC00000 (sign set).  Match the hardware for
// invalid-op defaults (inf-inf, 0*inf, ...); propagated source NaNs keep
// their sign/payload and are untouched (they never equal the default).
inline std::uint32_t norm_nan32(std::uint32_t r) {
  return r == 0xffc00000u ? 0x7fc00000u : r;
}

inline std::uint16_t flush16(std::uint16_t b) {
  if ((b & 0x7c00u) == 0 && (b & 0x03ffu)) return b & 0x8000u;
  return b;
}

}  // namespace

// ---- binary32 arithmetic (correctly rounded via Berkeley) --------------------
std::uint32_t sf_add32(std::uint32_t a, std::uint32_t b, FpPolicy policy) {
  if (policy.daz) { a = flush32(a); b = flush32(b); }
  set_round(policy);
  std::uint32_t r = bk32(f32_add(mk32(a), mk32(b)));
  r = norm_nan32(r);
  return policy.ftz ? flush32(r) : r;
}
std::uint32_t sf_sub32(std::uint32_t a, std::uint32_t b, FpPolicy policy) {
  if (policy.daz) { a = flush32(a); b = flush32(b); }
  set_round(policy);
  std::uint32_t r = bk32(f32_sub(mk32(a), mk32(b)));
  r = norm_nan32(r);
  return policy.ftz ? flush32(r) : r;
}
std::uint32_t sf_revsub32(std::uint32_t a, std::uint32_t b, FpPolicy policy) {
  if (policy.daz) { a = flush32(a); b = flush32(b); }
  set_round(policy);
  std::uint32_t r = bk32(f32_sub(mk32(b), mk32(a)));
  r = norm_nan32(r);
  return policy.ftz ? flush32(r) : r;
}
std::uint32_t sf_mul32(std::uint32_t a, std::uint32_t b, FpPolicy policy) {
  if (policy.daz) { a = flush32(a); b = flush32(b); }
  set_round(policy);
  std::uint32_t r = bk32(f32_mul(mk32(a), mk32(b)));
  r = norm_nan32(r);
  return policy.ftz ? flush32(r) : r;
}
std::uint32_t sf_sqrt32(std::uint32_t a, FpPolicy policy) {
  if (policy.daz) a = flush32(a);
  set_round(policy);
  std::uint32_t r = bk32(f32_sqrt(mk32(a)));
  return policy.ftz ? flush32(r) : r;
}

// ---- binary32 round-to-integer and fract --------------------------------------
std::uint32_t sf_floor32(std::uint32_t a, FpPolicy policy) {
  if (policy.daz) a = flush32(a);
  std::uint32_t r = bk32(f32_roundToInt(mk32(a), softfloat_round_min, false));
  return policy.ftz ? flush32(r) : r;
}
std::uint32_t sf_ceil32(std::uint32_t a, FpPolicy policy) {
  if (policy.daz) a = flush32(a);
  std::uint32_t r = bk32(f32_roundToInt(mk32(a), softfloat_round_max, false));
  return policy.ftz ? flush32(r) : r;
}
std::uint32_t sf_trunc32(std::uint32_t a, FpPolicy policy) {
  if (policy.daz) a = flush32(a);
  std::uint32_t r = bk32(f32_roundToInt(mk32(a), softfloat_round_minMag, false));
  return policy.ftz ? flush32(r) : r;
}
std::uint32_t sf_rndne32(std::uint32_t a, FpPolicy policy) {
  if (policy.daz) a = flush32(a);
  std::uint32_t r = bk32(f32_roundToInt(mk32(a), softfloat_round_near_even, false));
  return policy.ftz ? flush32(r) : r;
}
std::uint32_t sf_fract32(std::uint32_t a, FpPolicy policy) {
  // fract = a - floor(a) (fraction toward -inf), matching the GPU instruction.
  if (policy.daz) a = flush32(a);
  set_round(policy);
  const float32_t fl = f32_roundToInt(mk32(a), softfloat_round_min, false);
  std::uint32_t r = bk32(f32_sub(mk32(a), fl));
  return policy.ftz ? flush32(r) : r;
}

// ---- binary32 min/max/abs/neg/copysign (instruction bit rules) ----------------
std::uint32_t sf_min32(std::uint32_t a, std::uint32_t b, FpPolicy) {
  return minmax(a, b, true);
}
std::uint32_t sf_max32(std::uint32_t a, std::uint32_t b, FpPolicy) {
  return minmax(a, b, false);
}
std::uint32_t sf_min3(std::uint32_t a, std::uint32_t b, std::uint32_t c, FpPolicy) {
  return minmax(minmax(a, b, true), c, true);
}
std::uint32_t sf_max3(std::uint32_t a, std::uint32_t b, std::uint32_t c, FpPolicy) {
  return minmax(minmax(a, b, false), c, false);
}
std::uint32_t sf_med3(std::uint32_t a, std::uint32_t b, std::uint32_t c, FpPolicy) {
  const std::uint32_t lo = minmax(a, b, true);
  const std::uint32_t hi = minmax(a, b, false);
  return minmax(minmax(c, lo, false), hi, true);
}
std::uint32_t sf_abs32(std::uint32_t a, FpPolicy) { return a & ~kSign; }
std::uint32_t sf_neg32(std::uint32_t a, FpPolicy) { return a ^ kSign; }
std::uint32_t sf_copysign32(std::uint32_t a, std::uint32_t b, FpPolicy) {
  return (a & ~kSign) | (b & kSign);
}

// ---- integer <-> binary32 (RTNE via Berkeley; clamps keep exact semantics) ----
std::uint32_t sf_i32_f32(std::uint32_t a, FpPolicy policy) {
  if (a == 0x80000000u) {
    return 0xcf000000u;
  }
  const bool sign = (a & kSign) != 0;
  const std::uint64_t magnitude = sign ? (0u - a) : a;
  return sf_u32_f32(static_cast<std::uint32_t>(magnitude), policy) |
         (sign ? kSign : 0);
}
std::uint32_t sf_u32_f32(std::uint32_t a, FpPolicy) { return bk32(ui32_to_f32(a)); }

std::uint32_t sf_f32_i32(std::uint32_t a, FpPolicy) {
  const Decomposed d = decompose(a);
  if (d.is_nan) {
    return 0;
  }
  if (d.is_inf) {
    return d.sign ? 0x80000000u : 0x7fffffffu;
  }
  const std::uint32_t rounded = round_integer(a, FpPolicy::Rounding::TowardZero);
  const Decomposed dr = decompose(rounded);
  if (dr.is_zero) {
    return 0;
  }
  if (dr.exp >= 31) {
    return dr.sign ? 0x80000000u : 0x7fffffffu;
  }
  std::uint64_t value = dr.mant;
  if (dr.exp > 23) {
    value <<= static_cast<unsigned>(dr.exp - 23);
  } else if (dr.exp < 23) {
    value >>= static_cast<unsigned>(23 - dr.exp);
  }
  if (dr.sign) {
    value = 0u - value;
  }
  return static_cast<std::uint32_t>(value);
}

std::uint32_t sf_f32_u32(std::uint32_t a, FpPolicy) {
  const Decomposed d = decompose(a);
  if (d.is_nan || d.sign) {
    return 0;
  }
  if (d.is_inf) {
    return 0xffffffffu;
  }
  const std::uint32_t rounded = round_integer(a, FpPolicy::Rounding::TowardZero);
  const Decomposed dr = decompose(rounded);
  if (dr.is_zero) {
    return 0;
  }
  if (dr.exp >= 32) {
    return 0xffffffffu;
  }
  std::uint64_t value = dr.mant;
  if (dr.exp > 23) {
    value <<= static_cast<unsigned>(dr.exp - 23);
  } else if (dr.exp < 23) {
    value >>= static_cast<unsigned>(23 - dr.exp);
  }
  return static_cast<std::uint32_t>(value);
}

// ---- binary16 <-> binary32 ----------------------------------------------------
std::uint32_t sf_f32_f16(std::uint32_t a, FpPolicy policy) {
  if (policy.daz) a = flush32(a);
  set_round(policy);
  std::uint16_t r = bk16(f32_to_f16(mk32(a)));
  return policy.ftz ? flush16(r) : r;
}
std::uint32_t sf_f16_f32(std::uint32_t a, FpPolicy policy) {
  std::uint16_t h = static_cast<std::uint16_t>(a & 0xffffu);
  if (policy.daz) h = flush16(h);
  std::uint32_t r = bk32(f16_to_f32(mk16(h)));
  return policy.ftz ? flush32(r) : r;
}

// ---- binary64 arithmetic (correctly rounded via Berkeley) ----------------------
std::uint64_t sf_add64(std::uint64_t a, std::uint64_t b, FpPolicy policy) {
  if (policy.daz) { a = flush64(a); b = flush64(b); }
  set_round(policy);
  std::uint64_t r = bk64(f64_add(mk64(a), mk64(b)));
  return policy.ftz ? flush64(r) : r;
}
std::uint64_t sf_sub64(std::uint64_t a, std::uint64_t b, FpPolicy policy) {
  if (policy.daz) { a = flush64(a); b = flush64(b); }
  set_round(policy);
  std::uint64_t r = bk64(f64_sub(mk64(a), mk64(b)));
  return policy.ftz ? flush64(r) : r;
}
std::uint64_t sf_mul64(std::uint64_t a, std::uint64_t b, FpPolicy policy) {
  if (policy.daz) { a = flush64(a); b = flush64(b); }
  set_round(policy);
  std::uint64_t r = bk64(f64_mul(mk64(a), mk64(b)));
  return policy.ftz ? flush64(r) : r;
}
std::uint64_t sf_fma64(std::uint64_t a, std::uint64_t b, std::uint64_t c,
                       FpPolicy policy) {
  if (policy.daz) { a = flush64(a); b = flush64(b); c = flush64(c); }
  set_round(policy);
  std::uint64_t r = bk64(f64_mulAdd(mk64(a), mk64(b), mk64(c)));
  return policy.ftz ? flush64(r) : r;
}
std::uint64_t sf_sqrt64(std::uint64_t a, FpPolicy policy) {
  if (policy.daz) a = flush64(a);
  set_round(policy);
  std::uint64_t r = bk64(f64_sqrt(mk64(a)));
  return policy.ftz ? flush64(r) : r;
}
std::uint64_t sf_ldexp64(std::uint64_t a, std::uint32_t exp) {
  // [HW 2026-08-24] zero stays zero regardless of the exponent
  // (0 * 2^e = 0, no overflow to inf on the tested carrier).
  if ((a & 0x7fffffffffffffffull) == 0ull)
    return a;
  const D64 d = decompose64(a);
  if (d.is_nan || d.is_inf || d.is_zero) {
    return a;
  }
  // [HW 2026-08-24] int64 target so extreme 32-bit exponents
  // cannot wrap the int add.
  const int64_t target =
      static_cast<int64_t>(d.exp) + static_cast<int64_t>(static_cast<int32_t>(exp));
  if (target > 1023) {
    return (d.sign ? kS64 : 0) | 0x7ff0000000000000ull;
  }
  if (target < -1022) {
    return d.sign ? kS64 : 0;
  }
  // [HW 2026-08-24] the hardware keeps ~32 significand bits: any
  // result below min-normal x 2^-20 (52-bit mantissa content below
  // 2^32 in the low dword) flushes to signed zero.
  if (target == -1022 && d.mant < 0x100000000ull)
    return d.sign ? kS64 : 0;
  // [HW 2026-08-25] a nonzero denormal input with any nonzero shift
  // (e == 1 observed: target == -1021) also flushes to signed zero;
  // only e == 0 keeps the value (handled above).
  if ((a & 0x7ff0000000000000ull) == 0ull && d.mant != 0)
    return d.sign ? kS64 : 0;
  return compose64(d.sign, static_cast<int>(target), d.mant);
}
std::uint64_t sf_min64(std::uint64_t a, std::uint64_t b) {
  return minmax64(a, b, true);
}
std::uint64_t sf_max64(std::uint64_t a, std::uint64_t b) {
  return minmax64(a, b, false);
}

// ---- conversions between f32/f64 and integers (RTNE) --------------------------
std::uint64_t sf_f32_f64(std::uint32_t a) { return bk64(f32_to_f64(mk32(a))); }
std::uint32_t sf_f64_f32(std::uint64_t a) {
  set_round(FpPolicy{});
  return bk32(f64_to_f32(mk64(a)));
}
std::uint64_t sf_i32_f64(std::uint32_t a) { return bk64(i32_to_f64(static_cast<std::int32_t>(a))); }
std::uint64_t sf_u32_f64(std::uint32_t a) { return bk64(ui32_to_f64(a)); }

std::uint32_t sf_f64_i32(std::uint64_t a, FpPolicy) {
  const D64 d = decompose64(a);
  if (d.is_nan) {
    return 0;
  }
  if (d.is_inf) {
    return d.sign ? 0x80000000u : 0x7fffffffu;
  }
  if (d.is_zero) {
    return 0;
  }
  if (d.exp >= 31) {
    return d.sign ? 0x80000000u : 0x7fffffffu;
  }
  if (d.exp < 0) {
    return 0;
  }
  std::uint64_t value = d.mant;
  if (d.exp > 52) {
    value <<= static_cast<unsigned>(d.exp - 52);
  } else if (d.exp < 52) {
    value >>= static_cast<unsigned>(52 - d.exp);
  }
  if (d.sign) {
    value = 0u - value;
  }
  return static_cast<std::uint32_t>(value);
}

std::uint32_t sf_f64_u32(std::uint64_t a, FpPolicy) {
  const D64 d = decompose64(a);
  if (d.is_nan || d.sign) {
    return 0;
  }
  if (d.is_inf) {
    return 0xffffffffu;
  }
  if (d.is_zero) {
    return 0;
  }
  if (d.exp >= 32) {
    return 0xffffffffu;
  }
  if (d.exp < 0) {
    return 0;
  }
  std::uint64_t value = d.mant;
  if (d.exp > 52) {
    value <<= static_cast<unsigned>(d.exp - 52);
  } else if (d.exp < 52) {
    value >>= static_cast<unsigned>(52 - d.exp);
  }
  return static_cast<std::uint32_t>(value);
}

}  // namespace lgpu::test
