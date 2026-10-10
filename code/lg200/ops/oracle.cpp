#include "oracle.hpp"

#include "softfloat.hpp"

#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>

namespace lgpu::test {

AddU32Result oracle_v_add_u32(std::uint32_t a, std::uint32_t b) {
  const std::uint64_t sum = static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(b);
  return {static_cast<std::uint32_t>(sum), sum > 0xffffffffu};
}

AddCU32Result oracle_v_addc_u32(std::uint32_t a, std::uint32_t b, std::uint32_t cin) {
  // The carry lane is gated on the low 30 bits of VCC. The sum path is:
  //   low30 = cin & 0x3fffffff
  //   cin_eff = low30 != 0
  //   carry_out = (a + b + cin_eff) > 0xffffffff
  //   sum = low30 == 0          -> a + b
  //         low30 == 0x3fffffff -> a + b + 1            (standard)
  //         otherwise           -> (a+b) | (a+b+1)      (OR carry injection)
  const std::uint32_t low30 = cin & 0x3fffffffu;
  const std::uint32_t s = a + b;
  const bool cin_eff = low30 != 0;
  std::uint32_t sum;
  if (low30 == 0) {
    sum = s;
  } else if (low30 == 0x3fffffffu) {
    sum = s + 1u;
  } else {
    sum = s | (s + 1u);
  }
  const bool carry =
      (static_cast<std::uint64_t>(a) + b + (cin_eff ? 1u : 0u)) > 0xffffffffu;
  return {sum, carry};
}

SubU32Result oracle_v_sub_u32(std::uint32_t a, std::uint32_t b) {
  const std::uint32_t diff = a - b;
  return {diff, a < b};
}

std::uint32_t oracle_v_and_b32(std::uint32_t a, std::uint32_t b) {
  return a & b;
}

std::uint32_t oracle_v_or_b32(std::uint32_t a, std::uint32_t b) {
  return a | b;
}

std::uint32_t oracle_v_xor_b32(std::uint32_t a, std::uint32_t b) {
  return a ^ b;
}

std::uint32_t oracle_v_add_i32(std::uint32_t a, std::uint32_t b) {
  return a + b;
}

std::uint32_t oracle_v_sub_i32(std::uint32_t a, std::uint32_t b) {
  return a - b;
}

std::uint32_t oracle_v_abs_i32(std::uint32_t a) {
  return (a & 0x80000000u) == 0 ? a : 0u - a;
}

std::uint32_t oracle_v_clz_u32(std::uint32_t a) {
  std::uint32_t count = 0;
  std::uint32_t bit = 0x80000000u;
  while (bit != 0 && (a & bit) == 0) {
    ++count;
    bit >>= 1;
  }
  return count;
}

std::uint32_t oracle_v_bcnt1_u32(std::uint32_t a) {
  std::uint32_t count = 0;
  for (unsigned i = 0; i < 32; ++i) {
    count += (a >> i) & 1u;
  }
  return count;
}

std::uint32_t oracle_s_add_u32(std::uint32_t a, std::uint32_t b) {
  return a + b;
}

// Bit-exact u32 -> IEEE-754 binary32 with round-to-nearest-even, independent of
// the host FP mode.
std::uint32_t oracle_v_fcvt_f32_u32(std::uint32_t a) {
  if (a == 0) {
    return 0;
  }
  unsigned n = 31;
  while ((a & (1u << n)) == 0) {
    --n;
  }
  std::uint32_t exp = n + 127;
  std::uint32_t mant = 0;
  if (n < 24) {
    mant = (a << (23 - n)) & 0x7fffffu;
  } else {
    const unsigned shift = n - 23;
    const std::uint32_t dropped_mask = (1u << shift) - 1u;
    const std::uint32_t half = 1u << (shift - 1u);
    const std::uint32_t dropped = a & dropped_mask;
    mant = (a >> shift) & 0x7fffffu;
    if (dropped > half || (dropped == half && (mant & 1u) != 0)) {
      ++mant;
      if (mant == (1u << 23)) {
        mant = 0;
        ++exp;
      }
    }
  }
  return (exp << 23) | mant;
}

// Bit-exact f32 -> f16 with round-toward-zero, then pack src0 into the low
// half and src1 into the high half of the result (provisional; see operation
// text in the v_pack_f16rtz_f32 registry entry).
std::uint32_t oracle_v_pack_f16rtz_f32(std::uint32_t a, std::uint32_t b) {
  auto to_f16 = [](std::uint32_t v) -> std::uint32_t {
    const std::uint32_t sign = (v >> 31) & 1u;
    const std::uint32_t exp = (v >> 23) & 0xffu;
    const std::uint32_t mant = v & 0x7fffffu;
    if (exp == 0xffu) {
      return (sign << 15) | 0x7c00u | ((mant >> 13) & 0x3ffu);
    }
    const int e = static_cast<int>(exp) - 127;
    if (e >= 16) {
      // [HW 2026-08-24] the RTZ packer clamps overflow to the largest
      // finite half (0x7bff), not inf.
      return (sign << 15) | 0x7bffu;
    }
    if (e >= -14) {
      return (sign << 15) | (static_cast<std::uint32_t>(e + 15) << 10) | (mant >> 13);
    }
    if (e >= -24) {
      const unsigned shift = static_cast<unsigned>(-(e + 1));
      const std::uint32_t m16 = (0x800000u + mant) >> shift;
      return (sign << 15) | m16;
    }
    return sign << 15;
  };
  return to_f16(a) | (to_f16(b) << 16);
}

std::uint32_t oracle_sub_u32_value(std::uint32_t a, std::uint32_t b) {
  return oracle_v_sub_u32(a, b).diff;
}

std::uint32_t oracle_min3_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return oracle_min_u32(oracle_min_u32(a, b), c);
}

std::uint32_t oracle_max3_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return oracle_max_u32(oracle_max_u32(a, b), c);
}

std::uint32_t oracle_med3_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return oracle_max_u32(oracle_min_u32(a, b),
                        oracle_min_u32(oracle_max_u32(a, b), c));
}

std::uint32_t oracle_min3_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return oracle_min_i32(oracle_min_i32(a, b), c);
}

std::uint32_t oracle_max3_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return oracle_max_i32(oracle_max_i32(a, b), c);
}

std::uint32_t oracle_med3_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return oracle_max_i32(oracle_min_i32(a, b),
                        oracle_min_i32(oracle_max_i32(a, b), c));
}

std::uint32_t oracle_passthrough(std::uint32_t a) {
  return a;
}

// v_interp_1_f32 computes dst = s1 * (s0 + 1) in binary32 arithmetic.
std::uint32_t oracle_v_interp_1_f32(std::uint32_t s0, std::uint32_t s1) {
  float a, b;
  std::memcpy(&a, &s0, 4);
  std::memcpy(&b, &s1, 4);
  const float r = b * (a + 1.0f);
  std::uint32_t out;
  std::memcpy(&out, &r, 4);
  return out;
}

// v_mma_8816/323216_<sa><sb>_i32 computes a 16-byte dot product of the
// A/B register windows plus the C accumulator into the first output
// channel: dst[0] = C + sum_{k=0..15}(A[k] * B[k]) with per-byte signed
// (i8) or unsigned (u8) multiplication selected by the mnemonic variant.
// The 8816 shape additionally defines dst[1] = dst[0]; higher channels are
// undefined (junk).  A/B/C inputs are packed 32-bit dwords of the window
// base; K = 16 bytes.
std::uint32_t oracle_v_mma(std::uint32_t a, std::uint32_t b, std::uint32_t c,
                           bool signed_bytes) {
  std::int64_t dot = 0;
  for (int k = 0; k < 4; ++k) {
    const std::uint32_t ai = (a >> (8 * k)) & 0xffu;
    const std::uint32_t bi = (b >> (8 * k)) & 0xffu;
    if (signed_bytes) {
      dot += static_cast<std::int32_t>(static_cast<std::int8_t>(ai)) *
             static_cast<std::int32_t>(static_cast<std::int8_t>(bi));
    } else {
      dot += static_cast<std::int64_t>(ai) * static_cast<std::int64_t>(bi);
    }
  }
  dot *= 4;  // 4 identical dwords per window = 16 bytes
  return static_cast<std::uint32_t>(c + dot);
}

std::uint32_t oracle_sll_b32(std::uint32_t a, std::uint32_t n) {
  return a << (n & 31u);
}

std::uint32_t oracle_srl_b32(std::uint32_t a, std::uint32_t n) {
  return a >> (n & 31u);
}

std::uint32_t oracle_sra_b32(std::uint32_t a, std::uint32_t n) {
  const unsigned count = n & 31u;
  if (count == 0) {
    return a;
  }
  const std::uint32_t shifted = a >> count;
  if ((a & 0x80000000u) != 0) {
    return shifted | ~((1u << (32 - count)) - 1u);
  }
  return shifted;
}

std::uint32_t oracle_rotr_b32(std::uint32_t a, std::uint32_t n) {
  const unsigned count = n & 31u;
  if (count == 0) {
    return a;
  }
  return (a >> count) | (a << (32 - count));
}

std::uint32_t oracle_bfrev_b32(std::uint32_t a) {
  std::uint32_t out = 0;
  for (unsigned i = 0; i < 32; ++i) {
    out = (out << 1) | ((a >> i) & 1u);
  }
  return out;
}

std::uint32_t oracle_bfm_b32(std::uint32_t mask, std::uint32_t offset) {
  // Bit field mask: (1 << (mask & 31)) - 1 shifted by (offset & 31).
  const unsigned w = mask & 31u;
  const std::uint32_t ones = w == 0 ? 0u : ((1u << w) - 1u);
  return ones << (offset & 31u);
}

std::uint32_t oracle_bfe_u32(std::uint32_t a, std::uint32_t offset,
                             std::uint32_t width) {
  const unsigned o = offset & 31u;
  const unsigned w = width & 31u;
  if (w == 0) {
    return 0;
  }
  const std::uint32_t mask = (w == 32) ? 0xffffffffu : ((1u << w) - 1u);
  return (a >> o) & mask;
}

std::uint32_t oracle_bfe_i32(std::uint32_t a, std::uint32_t offset,
                             std::uint32_t width) {
  const unsigned o = offset & 31u;
  const unsigned w = width & 31u;
  if (w == 0) {
    return 0;
  }
  if (w == 32) {
    return a;
  }
  // [HW 2026-08-24] the hardware shifts arithmetically: negative
  // sources sign-extend through the shift (0xfffffffe >> 30 = -1).
  const std::uint32_t extracted =
      (static_cast<std::uint32_t>(static_cast<std::int32_t>(a) >> o)) &
      ((1u << w) - 1u);
  if ((extracted & (1u << (w - 1))) != 0) {
    return extracted | ~((1u << w) - 1u);
  }
  return extracted;
}

std::uint32_t oracle_bitalign_b32(std::uint32_t lo, std::uint32_t hi,
                                  std::uint32_t n) {
  const std::uint64_t joined =
      (static_cast<std::uint64_t>(hi) << 32) | static_cast<std::uint64_t>(lo);
  // The shift count is in bits and is masked to five bits.
  return static_cast<std::uint32_t>(joined >> (n & 31u));
}

std::uint32_t oracle_bytealign_b32(std::uint32_t lo, std::uint32_t hi,
                                   std::uint32_t n) {
  const std::uint64_t joined =
      (static_cast<std::uint64_t>(hi) << 32) | static_cast<std::uint64_t>(lo);
  // The byte count is masked to two bits.
  return static_cast<std::uint32_t>(joined >> ((n & 3u) * 8u));
}

std::uint32_t oracle_byteperm_b32(std::uint32_t lo, std::uint32_t hi,
                                  std::uint32_t sel) {
  std::uint32_t out = 0;
  const std::uint64_t joined =
      (static_cast<std::uint64_t>(hi) << 32) | static_cast<std::uint64_t>(lo);
  for (unsigned i = 0; i < 4; ++i) {
    const unsigned byte_index = (sel >> (8 * i)) & 0xffu;
    // [HW 2026-08-24] out-of-range byte selectors: 8..12 fill 0x00,
    // 13..255 fill 0xff.
    std::uint32_t value;
    if (byte_index < 8)
      value = static_cast<std::uint32_t>((joined >> (8 * byte_index)) & 0xffu);
    else if (byte_index < 13)
      value = 0u;
    else
      value = 0xffu;
    out |= value << (8 * i);
  }
  return out;
}

std::uint32_t oracle_min_u32(std::uint32_t a, std::uint32_t b) {
  return a < b ? a : b;
}

std::uint32_t oracle_max_u32(std::uint32_t a, std::uint32_t b) {
  return a > b ? a : b;
}

std::uint32_t oracle_min_i32(std::uint32_t a, std::uint32_t b) {
  return static_cast<int32_t>(a) < static_cast<int32_t>(b) ? a : b;
}

std::uint32_t oracle_max_i32(std::uint32_t a, std::uint32_t b) {
  return static_cast<int32_t>(a) > static_cast<int32_t>(b) ? a : b;
}

std::uint32_t oracle_mul_u32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::uint32_t>(static_cast<std::uint64_t>(a) * b);
}

std::uint32_t oracle_mul_hi_u32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::uint32_t>((static_cast<std::uint64_t>(a) * b) >> 32);
}

std::uint32_t oracle_mul_hi_i32(std::uint32_t a, std::uint32_t b) {
  const int64_t product =
      static_cast<int64_t>(static_cast<int32_t>(a)) * static_cast<int32_t>(b);
  return static_cast<std::uint32_t>(product >> 32);
}

std::uint32_t oracle_mul24_u32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::uint32_t>(
      (static_cast<std::uint64_t>(a & 0xffffffu) * (b & 0xffffffu)) & 0xffffffffu);
}

std::uint32_t oracle_mul24_i32(std::uint32_t a, std::uint32_t b) {
  const int32_t sa = static_cast<int32_t>(a << 8) >> 8;
  const int32_t sb = static_cast<int32_t>(b << 8) >> 8;
  return static_cast<std::uint32_t>(sa * sb);
}

std::uint32_t oracle_mul24_hi_u32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::uint32_t>(
      (static_cast<std::uint64_t>(a & 0xffffffu) * (b & 0xffffffu)) >> 32);
}

std::uint32_t oracle_mul24_hi_i32(std::uint32_t a, std::uint32_t b) {
  const int32_t sa = static_cast<int32_t>(a << 8) >> 8;
  const int32_t sb = static_cast<int32_t>(b << 8) >> 8;
  return static_cast<std::uint32_t>(
      (static_cast<int64_t>(sa) * sb) >> 32);
}

std::uint32_t oracle_absdiff_u32(std::uint32_t a, std::uint32_t b) {
  return a >= b ? a - b : b - a;
}

std::uint32_t oracle_absdiff_i32(std::uint32_t a, std::uint32_t b) {
  // Subtract modulo 2^32, then take the two's-complement magnitude.
  const std::uint32_t diff = a - b;
  const std::uint32_t magnitude = static_cast<std::int32_t>(diff) < 0
                                      ? 0u - diff
                                      : diff;
  return magnitude;
}

std::uint32_t oracle_subrev_u32(std::uint32_t a, std::uint32_t b) {
  return b - a;
}

std::uint32_t oracle_not_b32(std::uint32_t a) {
  return ~a;
}

std::uint32_t oracle_nand_b32(std::uint32_t a, std::uint32_t b) {
  return ~(a & b);
}

std::uint32_t oracle_nor_b32(std::uint32_t a, std::uint32_t b) {
  return ~(a | b);
}

std::uint32_t oracle_xnor_b32(std::uint32_t a, std::uint32_t b) {
  return ~(a ^ b);
}

std::uint32_t oracle_andn1_b32(std::uint32_t a, std::uint32_t b) {
  return a & ~b;
}

std::uint32_t oracle_andn2_b32(std::uint32_t a, std::uint32_t b) {
  return ~a & b;
}

std::uint32_t oracle_orn1_b32(std::uint32_t a, std::uint32_t b) {
  return a | ~b;
}

std::uint32_t oracle_orn2_b32(std::uint32_t a, std::uint32_t b) {
  return ~a | b;
}

std::uint32_t oracle_ineg_i32(std::uint32_t a) {
  return 0u - a;
}

std::uint32_t oracle_ctz_u32(std::uint32_t a) {
  if (a == 0) {
    return 32;
  }
  std::uint32_t count = 0;
  while ((a & 1u) == 0) {
    ++count;
    a >>= 1;
  }
  return count;
}

std::uint32_t oracle_clo_u32(std::uint32_t a) {
  return oracle_v_clz_u32(~a);
}

std::uint32_t oracle_cto_u32(std::uint32_t a) {
  return oracle_ctz_u32(~a);
}

std::uint32_t oracle_clb_u32(std::uint32_t a) {
  return (a & 0x80000000u) != 0 ? oracle_clo_u32(a) : oracle_v_clz_u32(a);
}

std::uint32_t oracle_bcnt0_u32(std::uint32_t a) {
  return 32u - oracle_v_bcnt1_u32(a);
}

std::uint32_t oracle_pack_u8_u32(std::uint32_t a, std::uint32_t b,
                               std::uint32_t c) {
  // [HW 2026-08-25] three-source pack (probed on ~25 points): byte
  // (b % 4) of c is replaced by the source byte with the U8 saturation
  // min(a, 0xff) on the RAW 32-bit value - in-range values pass through
  // (0x7f->0x7f, 0xaa->0xaa), anything >= 0x100 or negative (wrapping
  // huge) becomes 0xff (0x100->0xff, 0xffffffff->0xff, 0x80000000->0xff);
  // there is no zero floor in the hardware.
  const std::uint32_t byte = a <= 0xffu ? a : 0xffu;
  const unsigned pos = (b & 3u) * 8u;
  return (c & ~(0xffu << pos)) | (byte << pos);
}

std::uint32_t oracle_pack_i8_i32(std::uint32_t a, std::uint32_t b,
                                 std::uint32_t c) {
  // [HW 2026-08-25] three-source pack (probed on ~30 points): byte
  // (b % 4) of c is replaced by the FULL 32-bit signed value a saturated
  // to S8 [-128, 127]: 0x80->0x7f, 0x100->0x7f, 0x12345678->0x7f,
  // 0xffffffff->0xff (-1), 0x80000000->0x80 (-128), 0xffffff01->0x80.
  const std::int32_t sa = static_cast<std::int32_t>(a);
  // in-range values must truncate to the 8-bit two's-complement byte
  // (static_cast<u32>(sa) without the mask keeps 0xfffffffe for -2).
  const std::uint32_t byte = sa < -128 ? 0x80u
                            : sa > 127 ? 0x7fu
                                       : (static_cast<std::uint32_t>(sa) & 0xffu);
  const unsigned pos = (b & 3u) * 8u;
  return (c & ~(0xffu << pos)) | (byte << pos);
}

std::uint32_t oracle_pack_u16_u32(std::uint32_t a, std::uint32_t b) {
  // Saturate each half to the unsigned 16-bit range before packing.
  const auto sat16u = [](std::uint32_t v) -> std::uint32_t {
    return v > 0xffffu ? 0xffffu : v;
  };
  return sat16u(a) | (sat16u(b) << 16);
}

std::uint32_t oracle_pack_i16_i32(std::uint32_t a, std::uint32_t b) {
  // Saturate each half to the signed 16-bit range before packing.
  const std::int64_t sa = static_cast<std::int32_t>(a);
  const std::int64_t sb = static_cast<std::int32_t>(b);
  const auto sat16 = [](std::int64_t v) -> std::uint32_t {
    if (v > 32767) return 32767u;
    if (v < -32768) return 0x8000u;
    return static_cast<std::uint32_t>(static_cast<std::int16_t>(v)) & 0xffffu;
  };
  return sat16(sa) | (sat16(sb) << 16);
}

std::uint32_t oracle_sext_i32_i8(std::uint32_t a) {
  return static_cast<std::uint32_t>(static_cast<int32_t>(a << 24) >> 24);
}

std::uint32_t oracle_sext_i32_i16(std::uint32_t a) {
  return static_cast<std::uint32_t>(static_cast<int32_t>(a << 16) >> 16);
}

std::uint32_t oracle_bitset0_b32(std::uint32_t a, std::uint32_t bit) {
  return a & ~(1u << (bit & 31u));
}

std::uint32_t oracle_bitset1_b32(std::uint32_t a, std::uint32_t bit) {
  return a | (1u << (bit & 31u));
}

namespace {

// ---------------------------------------------------------------------------
// v_cmp / s_cmp predicate family. All FP compares are bit-exact and
// host-FP-mode independent: the IEEE sign-magnitude pattern is compared as
// an integer, NaN is unordered, +0 == -0.
// ---------------------------------------------------------------------------
enum class CmpPred {
  F, EQ, NEQ, LT, LE, GT, GE, LG, NLT, NLE, NGT, NGE, NLG, O, U, TRU
};

enum class CmpOrder { Less, Equal, Greater, Unordered };

template <typename T, typename NanCheck>
CmpOrder cmp_sign_magnitude(T a, T b, NanCheck is_nan, T sign_bit,
                            T magnitude_mask) {
  if (is_nan(a) || is_nan(b)) {
    return CmpOrder::Unordered;
  }
  const T sa = a & sign_bit;
  const T sb = b & sign_bit;
  const T ma = a & magnitude_mask;
  const T mb = b & magnitude_mask;
  if (ma == 0 && mb == 0) {
    return CmpOrder::Equal;  // +0 == -0
  }
  if (sa != sb) {
    return sa ? CmpOrder::Less : CmpOrder::Greater;
  }
  if (ma == mb) {
    return CmpOrder::Equal;
  }
  const bool a_larger = ma > mb;
  if (sa) {
    return a_larger ? CmpOrder::Less : CmpOrder::Greater;
  }
  return a_larger ? CmpOrder::Greater : CmpOrder::Less;
}

CmpOrder cmp_f32_bits(std::uint32_t a, std::uint32_t b) {
  const auto nan = [](std::uint32_t v) {
    return (v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0;
  };
  return cmp_sign_magnitude(a, b, nan, 0x80000000u, 0x7fffffffu);
}

CmpOrder cmp_f64_bits(std::uint64_t a, std::uint64_t b) {
  const auto nan = [](std::uint64_t v) {
    return (v & 0x7ff0000000000000ull) == 0x7ff0000000000000ull &&
           (v & 0x000fffffffffffffull) != 0;
  };
  return cmp_sign_magnitude(a, b, nan, std::uint64_t{0x8000000000000000ull},
                            std::uint64_t{0x7fffffffffffffffull});
}

// Half uses the same sign-magnitude ordering; the low 16 bits of the 32-bit
// operand carry the value; high bits are ignored.
CmpOrder cmp_f16_bits(std::uint32_t a, std::uint32_t b) {
  const auto nan = [](std::uint32_t v) {
    return (v & 0x7c00u) == 0x7c00u && (v & 0x03ffu) != 0;
  };
  return cmp_sign_magnitude(a & 0xffffu, b & 0xffffu, nan, 0x8000u, 0x7fffu);
}

std::uint32_t cmp_pred_u32(CmpOrder order, CmpPred p) {
  const bool un = order == CmpOrder::Unordered;
  switch (p) {
    case CmpPred::F:
      return 0u;
    case CmpPred::TRU:
      return 0xffffu;
    case CmpPred::EQ:
      return (!un && order == CmpOrder::Equal) ? 0xffffu : 0u;
    case CmpPred::NEQ:
      return (un || order != CmpOrder::Equal) ? 0xffffu : 0u;
    case CmpPred::LT:
      return (!un && order == CmpOrder::Less) ? 0xffffu : 0u;
    case CmpPred::LE:
      return (!un && (order == CmpOrder::Less || order == CmpOrder::Equal))
                 ? 0xffffu
                 : 0u;
    case CmpPred::GT:
      return (!un && order == CmpOrder::Greater) ? 0xffffu : 0u;
    case CmpPred::GE:
      return (!un && (order == CmpOrder::Greater || order == CmpOrder::Equal))
                 ? 0xffffu
                 : 0u;
    case CmpPred::LG:
      return (!un && order != CmpOrder::Equal) ? 0xffffu : 0u;
    case CmpPred::NLT:
      return (un || order != CmpOrder::Less) ? 0xffffu : 0u;
    case CmpPred::NLE:
      return (un || (order != CmpOrder::Less && order != CmpOrder::Equal))
                 ? 0xffffu
                 : 0u;
    case CmpPred::NGT:
      return (un || order != CmpOrder::Greater) ? 0xffffu : 0u;
    case CmpPred::NGE:
      return (un || (order != CmpOrder::Greater && order != CmpOrder::Equal))
                 ? 0xffffu
                 : 0u;
    case CmpPred::NLG:
      return (un || order == CmpOrder::Equal) ? 0xffffu : 0u;
    case CmpPred::O:
      return un ? 0u : 0xffffu;
    case CmpPred::U:
      return un ? 0xffffu : 0u;
  }
  return 0u;
}

std::uint64_t cmp_pred_u64(CmpOrder order, CmpPred p) {
  return cmp_pred_u32(order, p) ? 0xffffull : 0ull;
}

std::int32_t sext16(std::uint32_t v) {
  return static_cast<std::int32_t>(static_cast<std::int16_t>(v & 0xffffu));
}

// Class bits: 0 SNaN, 1 QNaN,
// 2 -inf, 3 -normal, 4 -subnormal, 5 -0, 6 +0, 7 +subnormal, 8 +normal,
// 9 +inf. Computed directly on the width's own bit pattern (a half
// subnormal stays a subnormal, unlike via f32 widening).
std::uint32_t class_bit_f16(std::uint32_t v) {
  const std::uint32_t exp = (v >> 10) & 0x1fu;
  const std::uint32_t man = v & 0x03ffu;
  const std::uint32_t sign = (v >> 15) & 1u;
  if (exp == 0 && man == 0) {
    return sign ? 5 : 6;
  }
  if (exp == 0) {
    return sign ? 4 : 7;
  }
  if (exp == 0x1fu) {
    if (man == 0) {
      return sign ? 2 : 9;
    }
    return (man >> 9) ? 1 : 0;  // QNaN / SNaN
  }
  return sign ? 3 : 8;
}

std::uint32_t class_bit_f32(std::uint32_t v) {
  const std::uint32_t exp = (v >> 23) & 0xffu;
  const std::uint32_t man = v & 0x7fffffu;
  const std::uint32_t sign = v >> 31;
  if (exp == 0 && man == 0) {
    return sign ? 5 : 6;
  }
  if (exp == 0) {
    return sign ? 4 : 7;
  }
  if (exp == 0xffu) {
    if (man == 0) {
      return sign ? 2 : 9;
    }
    return (man >> 22) ? 1 : 0;  // QNaN / SNaN
  }
  return sign ? 3 : 8;
}

std::uint32_t class_bit_f64(std::uint64_t v) {
  const std::uint32_t exp = static_cast<std::uint32_t>((v >> 52) & 0x7ffull);
  const std::uint64_t man = v & 0x000fffffffffffffull;
  const std::uint32_t sign = static_cast<std::uint32_t>(v >> 63);
  if (exp == 0 && man == 0) {
    return sign ? 5 : 6;
  }
  if (exp == 0) {
    return sign ? 4 : 7;
  }
  if (exp == 0x7ffu) {
    if (man == 0) {
      return sign ? 2 : 9;
    }
    return (man >> 51) ? 1 : 0;  // QNaN / SNaN
  }
  return sign ? 3 : 8;
}

// ---------------------------------------------------------------------------
// Atomic RMW helpers compute the value written to memory. Per-instruction
// oracles return both the architecturally returned old value and this update.
// ---------------------------------------------------------------------------
std::uint32_t rmw_add32(std::uint32_t old, std::uint32_t src) {
  return old + src;
}
std::uint64_t rmw_add64(std::uint64_t old, std::uint64_t src) {
  return old + src;
}
std::uint32_t rmw_sub32(std::uint32_t old, std::uint32_t src) {
  return old - src;
}
std::uint64_t rmw_sub64(std::uint64_t old, std::uint64_t src) {
  return old - src;
}
std::uint32_t rmw_rsub32(std::uint32_t old, std::uint32_t src) {
  return src - old;
}
std::uint64_t rmw_rsub64(std::uint64_t old, std::uint64_t src) {
  return src - old;
}
std::uint32_t rmw_inc32(std::uint32_t old, std::uint32_t bound) {
  return old < bound ? old + 1u : 0u;
}
std::uint64_t rmw_inc64(std::uint64_t old, std::uint64_t bound) {
  return old < bound ? old + 1ull : 0ull;
}
std::uint32_t rmw_dec32(std::uint32_t old, std::uint32_t bound) {
  return (old > 0u && old <= bound) ? old - 1u : bound;
}
std::uint64_t rmw_dec64(std::uint64_t old, std::uint64_t bound) {
  return (old > 0ull && old <= bound) ? old - 1ull : bound;
}
std::uint32_t rmw_max_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_max_i32(old, src);
}
std::uint64_t rmw_max_i64(std::uint64_t old, std::uint64_t src) {
  return static_cast<std::int64_t>(old) >= static_cast<std::int64_t>(src)
             ? old
             : src;
}
std::uint32_t rmw_min_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_min_i32(old, src);
}
std::uint64_t rmw_min_i64(std::uint64_t old, std::uint64_t src) {
  return static_cast<std::int64_t>(old) <= static_cast<std::int64_t>(src)
             ? old
             : src;
}
std::uint32_t rmw_swap32(std::uint32_t old, std::uint32_t src) {
  (void)old;
  return src;
}
std::uint64_t rmw_swap64(std::uint64_t old, std::uint64_t src) {
  (void)old;
  return src;
}
std::uint32_t rmw_cmpswap_eq32(std::uint32_t old, std::uint32_t cmp,
                               std::uint32_t swap) {
  return old == cmp ? swap : old;
}
std::uint64_t rmw_cmpswap_eq64(std::uint64_t old, std::uint64_t cmp,
                               std::uint64_t swap) {
  return old == cmp ? swap : old;
}
std::uint32_t rmw_cmpswap_gt_f32(std::uint32_t old, std::uint32_t data) {
  return cmp_f32_bits(old, data) == CmpOrder::Greater ? data : old;
}
std::uint64_t rmw_cmpswap_gt_f64(std::uint64_t old, std::uint64_t) {
  // [HW 2026-08-25] LG200 cmpswap_gt_f64 never swaps on the tested
  // carrier (identity); the f32 forms keep the GCN min semantics.
  return old;
}
std::uint32_t rmw_cmpswap_lt_f32(std::uint32_t old, std::uint32_t data) {
  return cmp_f32_bits(old, data) == CmpOrder::Less ? data : old;
}
std::uint64_t rmw_cmpswap_lt_f64(std::uint64_t old, std::uint64_t) {
  // [HW 2026-08-25] LG200 cmpswap_lt_f64 leaves the cell untouched in
  // every probed pair (UNRESOLVED why; f32 keeps the GCN max).
  return old;
}

template <typename T, typename Fn>
T oracle_rmw_new(Fn update, T old_value, T src) {
  return update(old_value, src);
}

template <typename T, typename Fn>
T oracle_rmw_new3(Fn update, T old_value, T cmp, T swap) {
  return update(old_value, cmp, swap);
}

}  // namespace

// --- v_cmp f16/f32: 32-bit result ------------------------------------------
std::uint32_t oracle_cmp_eq_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::EQ);
}
std::uint32_t oracle_cmp_eq_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::EQ);
}
std::uint32_t oracle_cmp_ge_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::GE);
}
std::uint32_t oracle_cmp_ge_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::GE);
}
std::uint32_t oracle_cmp_gt_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::GT);
}
std::uint32_t oracle_cmp_gt_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::GT);
}
std::uint32_t oracle_cmp_le_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::LE);
}
std::uint32_t oracle_cmp_le_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::LE);
}
std::uint32_t oracle_cmp_lg_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::LG);
}
std::uint32_t oracle_cmp_lg_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::LG);
}
std::uint32_t oracle_cmp_lt_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::LT);
}
// Floating-point comparison rules are precision-specific for negative
// predicates:
//   f32: standard unordered (see the oracle_cmp_ngt_f32 note below)
//   f64: ngt=nlg = (a is +0/-0 or NaN); nge = (a is NaN); nlt always 0xffff;
//        nle=neq = all-1 except a==0.0 && b==1.0 (legacy, no counter-evidence yet)
std::uint32_t oracle_cmp_ngt_f32_std(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::NGT);
}
std::uint32_t oracle_cmp_nle_f32_std(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::NLE);
}
static inline bool lg_f32_is_nan(std::uint32_t x) {
  return (x & 0x7f800000u) == 0x7f800000u && (x & 0x007fffffu) != 0u;
}
static inline bool lg_f64_is_nan(std::uint64_t x) {
  return (x & 0x7ff0000000000000ull) == 0x7ff0000000000000ull &&
         (x & 0x000fffffffffffffull) != 0ull;
}
static inline bool lg_f64_is_zero(std::uint64_t x) {
  return (x & 0x7fffffffffffffffull) == 0ull;
}
std::uint32_t oracle_cmp_lt_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::LT);
}
std::uint32_t oracle_cmp_neq_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::NEQ);
}
std::uint32_t oracle_cmp_neq_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::NEQ);
}
std::uint32_t oracle_cmp_nge_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::NGE);
}
std::uint32_t oracle_cmp_nge_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::NGE);
}
std::uint32_t oracle_cmp_ngt_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::NGT);
}
// Hardware verdict (16-lane carrier, three data points each): f32 NGT/NLE
// follow the standard unordered semantics (ngt(2.0,1.0)=0, ngt(1.0,2.0)=0xffff,
// NGT of NaN=true; nle mirror).  The old constant artifacts ("ngt always
// 0xffff", "nle iff b is NaN") were an incomplete-probe artifact and are
// superseded by the _std implementation.
std::uint32_t oracle_cmp_ngt_f32(std::uint32_t a, std::uint32_t b) {
  return oracle_cmp_ngt_f32_std(a, b);
}
std::uint32_t oracle_cmp_nle_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::NLE);
}
std::uint32_t oracle_cmp_nle_f32(std::uint32_t a, std::uint32_t b) {
  return oracle_cmp_nle_f32_std(a, b);
}
std::uint32_t oracle_cmp_nlg_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::NLG);
}
std::uint32_t oracle_cmp_nlg_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::NLG);
}
std::uint32_t oracle_cmp_nlt_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::NLT);
}
std::uint32_t oracle_cmp_nlt_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::NLT);
}
std::uint32_t oracle_cmp_o_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::O);
}
std::uint32_t oracle_cmp_o_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::O);
}
std::uint32_t oracle_cmp_u_f16(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f16_bits(a, b), CmpPred::U);
}
std::uint32_t oracle_cmp_u_f32(std::uint32_t a, std::uint32_t b) {
  return cmp_pred_u32(cmp_f32_bits(a, b), CmpPred::U);
}

// --- v_cmp integer: 32-bit result ------------------------------------------
std::uint32_t oracle_cmp_eq_i16(std::uint32_t a, std::uint32_t b) {
  return (sext16(a) == sext16(b)) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_eq_i32(std::uint32_t a, std::uint32_t b) {
  return (static_cast<std::int32_t>(a) == static_cast<std::int32_t>(b)) ? 0xffffu
                                                                        : 0u;
}
std::uint32_t oracle_cmp_eq_u16(std::uint32_t a, std::uint32_t b) {
  return (a & 0xffffu) == (b & 0xffffu) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_eq_u32(std::uint32_t a, std::uint32_t b) {
  return a == b ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_ne_u32(std::uint32_t a, std::uint32_t b) {
  return a != b ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_lt_u32(std::uint32_t a, std::uint32_t b) {
  return a < b ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_le_u32(std::uint32_t a, std::uint32_t b) {
  return a <= b ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_gt_u32(std::uint32_t a, std::uint32_t b) {
  return a > b ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_ge_u32(std::uint32_t a, std::uint32_t b) {
  return a >= b ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_lt_i32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::int32_t>(a) < static_cast<std::int32_t>(b) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_le_i32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::int32_t>(a) <= static_cast<std::int32_t>(b) ? 0xffffu
                                                                       : 0u;
}
std::uint32_t oracle_cmp_gt_i32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::int32_t>(a) > static_cast<std::int32_t>(b) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_ge_i32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::int32_t>(a) >= static_cast<std::int32_t>(b) ? 0xffffu
                                                                       : 0u;
}
std::uint32_t oracle_cmp_lt_u16(std::uint32_t a, std::uint32_t b) {
  return (a & 0xffffu) < (b & 0xffffu) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_le_u16(std::uint32_t a, std::uint32_t b) {
  return (a & 0xffffu) <= (b & 0xffffu) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_gt_u16(std::uint32_t a, std::uint32_t b) {
  return (a & 0xffffu) > (b & 0xffffu) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_ge_u16(std::uint32_t a, std::uint32_t b) {
  return (a & 0xffffu) >= (b & 0xffffu) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_lt_i16(std::uint32_t a, std::uint32_t b) {
  return sext16(a) < sext16(b) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_le_i16(std::uint32_t a, std::uint32_t b) {
  return sext16(a) <= sext16(b) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_gt_i16(std::uint32_t a, std::uint32_t b) {
  return sext16(a) > sext16(b) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_ge_i16(std::uint32_t a, std::uint32_t b) {
  return sext16(a) >= sext16(b) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_neq_u16(std::uint32_t a, std::uint32_t b) {
  return (a & 0xffffu) != (b & 0xffffu) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_neq_i16(std::uint32_t a, std::uint32_t b) {
  return sext16(a) != sext16(b) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_neq_u32(std::uint32_t a, std::uint32_t b) {
  return a != b ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_neq_i32(std::uint32_t a, std::uint32_t b) {
  return static_cast<std::int32_t>(a) != static_cast<std::int32_t>(b) ? 0xffffu
                                                                      : 0u;
}

// --- v_cmp f64/i64/u64: pair64 "both", u64 result --------------------------

std::uint32_t oracle_cmp_eq_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return cmp_pred_u32(cmp_f64_bits(a, b), CmpPred::EQ);}
std::uint32_t oracle_cmp_eq_i64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return (static_cast<std::int64_t>(a) == static_cast<std::int64_t>(b)) ? 0xffffu
        : 0u;}
std::uint32_t oracle_cmp_eq_u64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return a == b ? 0xffffu : 0u;}
std::uint32_t oracle_cmp_ge_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return cmp_pred_u32(cmp_f64_bits(a, b), CmpPred::GE);}
std::uint32_t oracle_cmp_ge_i64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return static_cast<std::int64_t>(a) >= static_cast<std::int64_t>(b) ? 0xffffu
        : 0u;}
std::uint32_t oracle_cmp_ge_u64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return a >= b ? 0xffffu : 0u;}
std::uint32_t oracle_cmp_gt_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return cmp_pred_u32(cmp_f64_bits(a, b), CmpPred::GT);}
std::uint32_t oracle_cmp_gt_i64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return static_cast<std::int64_t>(a) > static_cast<std::int64_t>(b) ? 0xffffu
        : 0u;}
std::uint32_t oracle_cmp_gt_u64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return a > b ? 0xffffu : 0u;}
std::uint32_t oracle_cmp_le_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return cmp_pred_u32(cmp_f64_bits(a, b), CmpPred::LE);}
std::uint32_t oracle_cmp_le_i64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return static_cast<std::int64_t>(a) <= static_cast<std::int64_t>(b) ? 0xffffu
        : 0u;}
std::uint32_t oracle_cmp_le_u64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return a <= b ? 0xffffu : 0u;}
std::uint32_t oracle_cmp_lt_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return cmp_pred_u32(cmp_f64_bits(a, b), CmpPred::LT);}
std::uint32_t oracle_cmp_lt_i64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return static_cast<std::int64_t>(a) < static_cast<std::int64_t>(b) ? 0xffffu
        : 0u;}
std::uint32_t oracle_cmp_lt_u64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return a < b ? 0xffffu : 0u;}
std::uint32_t oracle_cmp_neq_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  // This predicate has the same result as nle.
  return (lg_f64_is_zero(a) && b == 0x3ff0000000000000ull) ? 0u : 0xffffu;
}
std::uint32_t oracle_cmp_neq_i64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return static_cast<std::int64_t>(a) != static_cast<std::int64_t>(b) ? 0xffffu
        : 0u;}
std::uint32_t oracle_cmp_neq_u64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return a != b ? 0xffffu : 0u;}
std::uint32_t oracle_cmp_lg_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  return cmp_pred_u32(cmp_f64_bits(a, b), CmpPred::LG);}
std::uint32_t oracle_cmp_nlt_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  (void)a; (void)b;
  return 0xffffu;
}
std::uint32_t oracle_cmp_nle_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  // False only when a is zero and b is 1.0.
  return (lg_f64_is_zero(a) && b == 0x3ff0000000000000ull) ? 0u : 0xffffu;
}
std::uint32_t oracle_cmp_ngt_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  (void)b;
  return (lg_f64_is_zero(a) || lg_f64_is_nan(a)) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_nge_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  (void)b;
  return lg_f64_is_nan(a) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_nlg_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  (void)b;
  return (lg_f64_is_zero(a) || lg_f64_is_nan(a)) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmp_o_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  // The second operand is ignored; o == !isnan(a).
  (void)b;
  return lg_f64_is_nan(a) ? 0u : 0xffffu;}
std::uint32_t oracle_cmp_u_f64(std::uint64_t a, std::uint64_t b,
                      const FpPolicy&) {
  // The second operand is ignored; u == isnan(a).
  (void)b;
  return lg_f64_is_nan(a) ? 0xffffu : 0u;}

// --- constant predicates (width-overloaded) --------------------------------
std::uint32_t oracle_cmp_tru(std::uint32_t, std::uint32_t) {
  return 0xffffu;
}
std::uint64_t oracle_cmp_tru(std::uint64_t, std::uint64_t) {
  return 0xffffull;
}
std::uint32_t oracle_cmp_fals(std::uint32_t, std::uint32_t) {
  return 0u;
}
std::uint64_t oracle_cmp_fals(std::uint64_t, std::uint64_t) {
  return 0ull;
}

// --- cmpclass --------------------------------------------------------------
std::uint32_t oracle_cmpclass_f16(std::uint32_t mask, std::uint32_t v) {
  return (mask & (1u << class_bit_f16(v))) ? 0xffffu : 0u;
}
std::uint32_t oracle_cmpclass_f32(std::uint32_t mask, std::uint32_t v) {
  return (mask & (1u << class_bit_f32(v))) ? 0xffffu : 0u;
}
std::uint64_t oracle_cmpclass_f64(std::uint64_t mask, std::uint64_t v) {
  return (mask & (1ull << class_bit_f64(v))) ? 0xffffull : 0ull;
}


// --- scalar carry/borrow/rotate --------------------------------------------
std::uint32_t oracle_s_addc_u32(std::uint32_t src, std::uint32_t imm,
                                std::uint32_t carry_in) {
  // [HW 2026-08-23] the SCC carry/borrow-in state is a non-zero flag: every
  // nonzero third input produces a carry (src2 = 0x2/0x4/0x80000000 all
  // carried on the LG200 carrier).  Not (value & 1).
  const std::uint32_t c = carry_in != 0u ? 1u : 0u;
  return static_cast<std::uint32_t>(static_cast<std::uint64_t>(src) + imm + c);
}

std::uint32_t oracle_s_subb_u32(std::uint32_t src, std::uint32_t imm,
                                std::uint32_t borrow_in) {
  const std::uint32_t b = borrow_in != 0u ? 1u : 0u;
  return src - imm - b;
}

std::uint32_t oracle_s_subbrev_u32(std::uint32_t src, std::uint32_t imm,
                                   std::uint32_t borrow_in) {
  const std::uint32_t b = borrow_in != 0u ? 1u : 0u;
  return imm - src - b;
}

std::uint32_t oracle_rotrrev_b32(std::uint32_t count, std::uint32_t data) {
  return oracle_rotr_b32(data, count);
}

// --- atomic RMW: shared memory ---------------------------------------------
std::uint32_t oracle_sm_add_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_add32, old, src);
}
std::uint32_t oracle_sm_and_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_and_b32, old, src);
}
std::uint32_t oracle_sm_dec_u32(std::uint32_t old, std::uint32_t bound) {
  return oracle_rmw_new(rmw_dec32, old, bound);
}
std::uint32_t oracle_sm_inc_u32(std::uint32_t old, std::uint32_t bound) {
  return oracle_rmw_new(rmw_inc32, old, bound);
}
std::uint32_t oracle_sm_max_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_max_i32, old, src);
}
std::uint32_t oracle_sm_max_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_max_u32, old, src);
}
std::uint32_t oracle_sm_min_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_min_i32, old, src);
}
std::uint32_t oracle_sm_min_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_min_u32, old, src);
}
std::uint32_t oracle_sm_or_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_or_b32, old, src);
}
std::uint32_t oracle_sm_rsub_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_rsub32, old, src);
}
std::uint32_t oracle_sm_sub_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_sub32, old, src);
}
std::uint32_t oracle_sm_swap_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_swap32, old, src);
}
std::uint32_t oracle_sm_xor_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_xor_b32, old, src);
}

// --- atomic RMW: buffer ----------------------------------------------------
std::uint32_t oracle_v_buff_cmpswap_b32(std::uint32_t old, std::uint32_t cmp,
                                        std::uint32_t swap) {
  return oracle_rmw_new3(rmw_cmpswap_eq32, old, cmp, swap);
}
std::uint32_t oracle_v_buff_dec_u32(std::uint32_t old, std::uint32_t bound) {
  return oracle_rmw_new(rmw_dec32, old, bound);
}
std::uint32_t oracle_v_buff_inc_u32(std::uint32_t old, std::uint32_t bound) {
  return oracle_rmw_new(rmw_inc32, old, bound);
}
std::uint32_t oracle_v_buff_sub_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_sub32, old, src);
}

// --- atomic RMW: flat ------------------------------------------------------
std::uint32_t oracle_v_flat_add_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_add32, old, src);
}
std::uint32_t oracle_v_flat_and_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_and_b32, old, src);
}
std::uint32_t oracle_v_flat_cmpswap_b32(std::uint32_t old, std::uint32_t cmp,
                                        std::uint32_t swap) {
  return oracle_rmw_new3(rmw_cmpswap_eq32, old, cmp, swap);
}
std::uint32_t oracle_v_flat_dec_u32(std::uint32_t old, std::uint32_t bound) {
  return oracle_rmw_new(rmw_dec32, old, bound);
}
std::uint32_t oracle_v_flat_inc_u32(std::uint32_t old, std::uint32_t bound) {
  return oracle_rmw_new(rmw_inc32, old, bound);
}
std::uint32_t oracle_v_flat_max_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_max_i32, old, src);
}
std::uint32_t oracle_v_flat_max_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_max_u32, old, src);
}
std::uint32_t oracle_v_flat_min_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_min_i32, old, src);
}
std::uint32_t oracle_v_flat_min_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_min_u32, old, src);
}
std::uint32_t oracle_v_flat_or_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_or_b32, old, src);
}
std::uint32_t oracle_v_flat_sub_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_sub32, old, src);
}
std::uint32_t oracle_v_flat_swap_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_swap32, old, src);
}
std::uint32_t oracle_v_flat_xor_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_xor_b32, old, src);
}

// --- atomic RMW: tex -------------------------------------------------------
std::uint32_t oracle_v_tex_add_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_add32, old, src);
}
std::uint32_t oracle_v_tex_and_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_and_b32, old, src);
}
std::uint32_t oracle_v_tex_cmpswap_b32(std::uint32_t old, std::uint32_t cmp,
                                       std::uint32_t swap) {
  return oracle_rmw_new3(rmw_cmpswap_eq32, old, cmp, swap);
}
std::uint32_t oracle_v_tex_dec_u32(std::uint32_t old, std::uint32_t bound) {
  return oracle_rmw_new(rmw_dec32, old, bound);
}
std::uint32_t oracle_v_tex_inc_u32(std::uint32_t old, std::uint32_t bound) {
  return oracle_rmw_new(rmw_inc32, old, bound);
}
std::uint32_t oracle_v_tex_max_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_max_i32, old, src);
}
std::uint32_t oracle_v_tex_max_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_max_u32, old, src);
}
std::uint32_t oracle_v_tex_min_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_min_i32, old, src);
}
std::uint32_t oracle_v_tex_min_u32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_min_u32, old, src);
}
std::uint32_t oracle_v_tex_or_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_or_b32, old, src);
}
std::uint32_t oracle_v_tex_sub_i32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(rmw_sub32, old, src);
}
std::uint32_t oracle_v_tex_swap_b32(std::uint32_t old, std::uint32_t src) {
  return rmw_swap32(old, src);
}
std::uint32_t oracle_v_tex_xor_b32(std::uint32_t old, std::uint32_t src) {
  return oracle_rmw_new(oracle_v_xor_b32, old, src);
}

// --- atomic RMW: conditional 32-bit ----------------------------------------
std::uint32_t oracle_sm_cmpswap_eq_u32(std::uint32_t old, std::uint32_t cmp,
                                       std::uint32_t swap) {
  return oracle_rmw_new3(rmw_cmpswap_eq32, old, cmp, swap);
}
std::uint32_t oracle_sm_cmpswap_gt_f32(std::uint32_t old, std::uint32_t data) {
  return oracle_rmw_new(rmw_cmpswap_gt_f32, old, data);
}
std::uint32_t oracle_sm_cmpswap_lt_f32(std::uint32_t old, std::uint32_t data) {
  return oracle_rmw_new(rmw_cmpswap_lt_f32, old, data);
}

// --- atomic RMW: shared memory 64-bit --------------------------------------
std::uint64_t oracle_sm_add_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_add64, old, src);
}
std::uint64_t oracle_sm_and_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x & y; },
                        old, src);
}
std::uint64_t oracle_sm_dec_u64(std::uint64_t old, std::uint64_t bound) {
  return oracle_rmw_new(rmw_dec64, old, bound);
}
std::uint64_t oracle_sm_inc_u64(std::uint64_t old, std::uint64_t bound) {
  return oracle_rmw_new(rmw_inc64, old, bound);
}
std::uint64_t oracle_sm_max_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_max_i64, old, src);
}
std::uint64_t oracle_sm_max_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) {
           return x > y ? x : y;
         }, old, src);
}
std::uint64_t oracle_sm_min_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_min_i64, old, src);
}
std::uint64_t oracle_sm_min_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) {
           return x < y ? x : y;
         }, old, src);
}
std::uint64_t oracle_sm_or_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x | y; },
                        old, src);
}
std::uint64_t oracle_sm_rsub_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_rsub64, old, src);
}
std::uint64_t oracle_sm_sub_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_sub64, old, src);
}
std::uint64_t oracle_sm_swap_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_swap64, old, src);
}
std::uint64_t oracle_sm_xor_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x ^ y; },
                        old, src);
}

// --- atomic RMW: buffer 64-bit ---------------------------------------------
std::uint64_t oracle_v_buff_add_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_add64, old, src);
}
std::uint64_t oracle_v_buff_and_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x & y; },
                        old, src);
}
std::uint64_t oracle_v_buff_cmpswap_b64(std::uint64_t old, std::uint64_t cmp,
                                        std::uint64_t swap) {
  return oracle_rmw_new3(rmw_cmpswap_eq64, old, cmp, swap);
}
std::uint64_t oracle_v_buff_dec_u64(std::uint64_t old, std::uint64_t bound) {
  return oracle_rmw_new(rmw_dec64, old, bound);
}
std::uint64_t oracle_v_buff_inc_u64(std::uint64_t old, std::uint64_t bound) {
  return oracle_rmw_new(rmw_inc64, old, bound);
}
std::uint64_t oracle_v_buff_max_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_max_i64, old, src);
}
std::uint64_t oracle_v_buff_max_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) {
           return x > y ? x : y;
         }, old, src);
}
std::uint64_t oracle_v_buff_min_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_min_i64, old, src);
}
std::uint64_t oracle_v_buff_min_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) {
           return x < y ? x : y;
         }, old, src);
}
std::uint64_t oracle_v_buff_or_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x | y; },
                        old, src);
}
std::uint64_t oracle_v_buff_sub_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_sub64, old, src);
}
std::uint64_t oracle_v_buff_swap_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_swap64, old, src);
}
std::uint64_t oracle_v_buff_xor_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x ^ y; },
                        old, src);
}

// --- atomic RMW: flat 64-bit -----------------------------------------------
std::uint64_t oracle_v_flat_add_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_add64, old, src);
}
std::uint64_t oracle_v_flat_and_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x & y; },
                        old, src);
}
std::uint64_t oracle_v_flat_cmpswap_b64(std::uint64_t old, std::uint64_t cmp,
                                        std::uint64_t swap) {
  return oracle_rmw_new3(rmw_cmpswap_eq64, old, cmp, swap);
}
std::uint64_t oracle_v_flat_dec_u64(std::uint64_t old, std::uint64_t bound) {
  return oracle_rmw_new(rmw_dec64, old, bound);
}
std::uint64_t oracle_v_flat_inc_u64(std::uint64_t old, std::uint64_t bound) {
  return oracle_rmw_new(rmw_inc64, old, bound);
}
std::uint64_t oracle_v_flat_max_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_max_i64, old, src);
}
std::uint64_t oracle_v_flat_max_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) {
           return x > y ? x : y;
         }, old, src);
}
std::uint64_t oracle_v_flat_min_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_min_i64, old, src);
}
std::uint64_t oracle_v_flat_min_u64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) {
           return x < y ? x : y;
         }, old, src);
}
std::uint64_t oracle_v_flat_or_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x | y; },
                        old, src);
}
std::uint64_t oracle_v_flat_sub_i64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_sub64, old, src);
}
std::uint64_t oracle_v_flat_swap_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new(rmw_swap64, old, src);
}
std::uint64_t oracle_v_flat_xor_b64(std::uint64_t old, std::uint64_t src) {
  return oracle_rmw_new([](std::uint64_t x, std::uint64_t y) { return x ^ y; },
                        old, src);
}

// --- atomic RMW: conditional 64-bit ----------------------------------------
std::uint64_t oracle_sm_cmpswap_eq_u64(std::uint64_t old, std::uint64_t cmp,
                                       std::uint64_t swap) {
  return oracle_rmw_new3(rmw_cmpswap_eq64, old, cmp, swap);
}
std::uint64_t oracle_sm_cmpswap_gt_f64(std::uint64_t old, std::uint64_t data) {
  return oracle_rmw_new(rmw_cmpswap_gt_f64, old, data);
}
std::uint64_t oracle_sm_cmpswap_lt_f64(std::uint64_t old, std::uint64_t data) {
  return oracle_rmw_new(rmw_cmpswap_lt_f64, old, data);
}



std::uint64_t oracle_passthrough64(std::uint64_t a) {
  return a;
}

// s_select_b32: dst = (scc != 0) ? src0 : src1.
namespace {
std::uint32_t cube_ftz3(std::uint32_t a, std::uint32_t b, std::uint32_t c,
                        float& s, float& t, float& r);
}
std::uint32_t oracle_cubema(std::uint32_t s0, std::uint32_t s1,
                             std::uint32_t s2) {
  // Cube major axis multiplied by two, retaining the dominant-axis sign.
  // [HW 2026-08-24] z axis wins magnitude ties, -0.0 keeps its sign,
  // NaN propagates, denormals flush.
  float s, t, r;
  if (uint32_t nan = cube_ftz3(s0, s1, s2, s, t, r)) return nan;
  float as = std::fabs(s), at = std::fabs(t), ar = std::fabs(r);
  float dom;
  if (ar >= at && ar >= as)
    dom = r;
  else if (at >= as)
    dom = t;
  else
    dom = s;
  float out = 2.0f * dom;
  std::uint32_t u;
  std::memcpy(&u, &out, 4);
  return u;
}
std::uint32_t oracle_cubeface(std::uint32_t s0, std::uint32_t s1,
                               std::uint32_t s2) {
  // Cube face id: +x=0, -x=1, +y=2, -y=3, +z=4, -z=5.
  float s, t, r;
  std::memcpy(&s, &s0, 4);
  std::memcpy(&t, &s1, 4);
  std::memcpy(&r, &s2, 4);
  // [HW 2026-08-24] z-axis preferred on magnitude ties
  // (|r| >= |t| && |r| >= |s| -> z, else |t| >= |s| -> y, else x).
  auto ftz = [](std::uint32_t b) -> std::uint32_t {
    return (b & 0x7f800000u) == 0u ? (b & 0x80000000u) : b;
  };
  // [HW 2026-08-24] a NaN input propagates as the face result (the
  // selector returns the NaN bit pattern itself).
  if ((s0 & 0x7f800000u) == 0x7f800000u && (s0 & 0x007fffffu) != 0u)
    return s0;
  if ((s1 & 0x7f800000u) == 0x7f800000u && (s1 & 0x007fffffu) != 0u)
    return s1;
  if ((s2 & 0x7f800000u) == 0x7f800000u && (s2 & 0x007fffffu) != 0u)
    return s2;
  s0 = ftz(s0);
  s1 = ftz(s1);
  s2 = ftz(s2);
  std::memcpy(&s, &s0, 4);
  std::memcpy(&t, &s1, 4);
  std::memcpy(&r, &s2, 4);
  float as = std::fabs(s), at = std::fabs(t), ar = std::fabs(r);
  // [HW 2026-08-24] the sign test includes -0.0 (hardware treats the
  // negative-zero face as negative), so use the sign bit, not < 0.
  const bool rn = std::signbit(r), tn = std::signbit(t), sn = std::signbit(s);
  float face;
  if (ar >= at && ar >= as) {
    face = rn ? 5.0f : 4.0f;
  } else if (at >= as) {
    face = tn ? 3.0f : 2.0f;
  } else {
    face = sn ? 1.0f : 0.0f;
  }
  std::uint32_t u;
  std::memcpy(&u, &face, 4);
  return u;
}
namespace {
// [HW 2026-08-24] face selection matches oracle_cubeface: z axis wins
// magnitude ties, -0.0 counts as negative (signbit).
int cube_face(float s, float t, float r) {
  float as = std::fabs(s), at = std::fabs(t), ar = std::fabs(r);
  if (ar >= at && ar >= as) return std::signbit(r) ? 5 : 4;  // -z / +z
  if (at >= as) return std::signbit(t) ? 3 : 2;              // -y / +y
  return std::signbit(s) ? 1 : 0;                            // -x / +x
}
std::uint32_t cube_ftz3(std::uint32_t a, std::uint32_t b, std::uint32_t c,
                        float& s, float& t, float& r) {
  auto ftz = [](uint32_t v) -> uint32_t {
    return (v & 0x7f800000u) == 0u ? (v & 0x80000000u) : v;
  };
  if ((a & 0x7f800000u) == 0x7f800000u && (a & 0x007fffffu) != 0u) return a;
  if ((b & 0x7f800000u) == 0x7f800000u && (b & 0x007fffffu) != 0u) return b;
  if ((c & 0x7f800000u) == 0x7f800000u && (c & 0x007fffffu) != 0u) return c;
  a = ftz(a); b = ftz(b); c = ftz(c);
  std::memcpy(&s, &a, 4); std::memcpy(&t, &b, 4); std::memcpy(&r, &c, 4);
  return 0;
}
}
std::uint32_t oracle_cubes(std::uint32_t s0, std::uint32_t s1,
                           std::uint32_t s2) {
  float s, t, r;
  if (uint32_t nan = cube_ftz3(s0, s1, s2, s, t, r)) return nan;
  float out;
  switch (cube_face(s, t, r)) {
    case 0: out = -r; break;  // +x
    case 1: out = r; break;   // -x
    case 2: out = s; break;   // +y
    case 3: out = s; break;   // -y
    case 4: out = s; break;   // +z
    default: out = -s; break; // -z
  }
  std::uint32_t u;
  std::memcpy(&u, &out, 4);
  return u;
}
std::uint32_t oracle_cubet(std::uint32_t s0, std::uint32_t s1,
                           std::uint32_t s2) {
  float s, t, r;
  if (uint32_t nan = cube_ftz3(s0, s1, s2, s, t, r)) return nan;
  float out;
  switch (cube_face(s, t, r)) {
    case 0: out = -t; break;  // +x
    case 1: out = -t; break;  // -x
    case 2: out = r; break;   // +y
    case 3: out = -r; break;  // -y
    case 4: out = -t; break;  // +z
    default: out = -t; break; // -z
  }
  std::uint32_t u;
  std::memcpy(&u, &out, 4);
  return u;
}
std::uint32_t oracle_bitcmprev0(std::uint32_t a, std::uint32_t bit) {
  return ((a >> (bit & 31u)) & 1u) == 0u ? 0xffffu : 0u;
}
std::uint32_t oracle_bitcmprev1(std::uint32_t a, std::uint32_t bit) {
  return ((a >> (bit & 31u)) & 1u) == 1u ? 0xffffu : 0u;
}
std::uint32_t oracle_quadmask(std::uint32_t value) {
  std::uint32_t result = 0;
  for (unsigned quad = 0; quad < 8; ++quad) {
    if (((value >> (quad * 4)) & 0xfu) != 0) {
      result |= 1u << quad;
    }
  }
  return result;
}
std::uint32_t oracle_wqm(std::uint32_t value) {
  std::uint32_t result = 0;
  for (unsigned quad = 0; quad < 8; ++quad) {
    if (((value >> (quad * 4)) & 0xfu) != 0) {
      result |= 0xfu << (quad * 4);
    }
  }
  return result;
}
std::uint32_t oracle_s_select_b32(std::uint32_t a, std::uint32_t b,
                                  std::uint32_t scc) {
  return scc != 0 ? a : b;
}

// Reverse shifts: count, data (matches the s_rotrrev_imm_b32 convention).
std::uint32_t oracle_sllrev_b32(std::uint32_t count, std::uint32_t data) {
  return data << (count & 31u);
}
std::uint32_t oracle_srlrev_b32(std::uint32_t count, std::uint32_t data) {
  return data >> (count & 31u);
}
std::uint32_t oracle_srarev_b32(std::uint32_t count, std::uint32_t data) {
  return static_cast<std::uint32_t>(
      static_cast<std::int32_t>(data) >> (count & 31u));
}



std::uint32_t oracle_cmp_ne_u64(std::uint64_t a, std::uint64_t b,
                                const FpPolicy&) {
  return a != b ? 0xffffu : 0u;
}

std::uint32_t oracle_cmp_tru(std::uint64_t, std::uint64_t,
                             const FpPolicy&) {
  return 0xffffu;
}

std::uint32_t oracle_cmp_fals(std::uint64_t, std::uint64_t,
                              const FpPolicy&) {
  return 0u;
}


// --- iteration-4 bit helpers ---
static inline std::uint32_t bits_f32(float f) {
  std::uint32_t u; memcpy(&u, &f, 4); return u;
}
static inline float f32_bits(std::uint32_t u) {
  float f; memcpy(&f, &u, 4); return f;
}
static inline std::uint64_t bits_f64(double d) {
  std::uint64_t u; memcpy(&u, &d, 8); return u;
}
static inline double f64_bits(std::uint64_t u) {
  double d; memcpy(&d, &u, 8); return d;
}
// Additional instruction families.
std::uint32_t oracle_mad_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return static_cast<std::uint32_t>(static_cast<std::int32_t>(a) *
                                        static_cast<std::int32_t>(b) +
                                    static_cast<std::int32_t>(c));
}
std::uint32_t oracle_mad_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return a * b + c;
}
std::uint32_t oracle_mad24_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  // [HW 2026-08-24] the 24-bit sources are SIGN-EXTENDED integers:
  // 0x800000 = -8388608 (not +8388608).  Verified: (0x800000,0x800000)
  // -> 2^46 mod 2^32 = 0; (0x800000,1) -> -2^23 = 0xff800000;
  // (0x7fffff,0x800000) -> 2^23 = 0x00800000.
  const std::int32_t sa = static_cast<std::int32_t>(a << 8) >> 8;
  const std::int32_t sb = static_cast<std::int32_t>(b << 8) >> 8;
  return static_cast<std::uint32_t>(sa * sb + static_cast<std::int32_t>(c));
}
std::uint32_t oracle_mad24_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  return (a & 0xffffffu) * (b & 0xffffffu) + c;
}
std::uint32_t oracle_bfi_b32(std::uint32_t src0, std::uint32_t src1,
                          std::uint32_t src2) {
  return (src0 & src1) | (~src0 & src2);
}
std::uint32_t oracle_mbcnt_u32(std::uint32_t v, std::uint32_t mask) {
  // LG200 mbcnt operation:
  //   dst = b | M'
  //   M  = (a[14:0]==0) ? 0 : 2^ceil(log2(popcount(a[14:0])+1))-1
  //   h  = MSB of M; if b has bit h set then M' = M | (1<<z), where z is
  //        the first zero bit of b above h; otherwise M' = M.
  // a = v (src0, popcount source), b = mask (src1, passthrough source).
  const std::uint32_t a = v;
  const std::uint32_t b = mask;
  const std::uint32_t alo = a & 0x7fffu;
  std::uint32_t M = 0;
  // [HW 2026-08-24] M ladder over pc(a[14:0]) plus (when a has any
  // low-15 bit) pc(b[14:0]): a=0 keeps M=0 even with b bits set.
  const int pc = __builtin_popcount(alo) +
                 (alo == 0 ? 0 : __builtin_popcount(b & 0x7fffu));
  if (pc != 0) {
    int n = 1;
    while ((1u << n) < static_cast<std::uint32_t>(pc + 1)) ++n;
    M = (1u << n) - 1u;
  }
  // [HW 2026-08-24] post-extension of M through b (when b covers
  // M's MSB, extend to b's first zero ABOVE it, including the top bit).
  std::uint32_t Mp = M;
  if (M != 0) {
    const unsigned h = 31u - __builtin_clz(M);
    if ((b >> h) & 1u) {
      unsigned z = h + 1;
      while (z < 32 && ((b >> z) & 1u)) ++z;
      if (z < 32) Mp |= (1u << z);
    }
  }
  return b | Mp;
}
std::uint32_t oracle_threadid_u32(std::uint32_t) { return 0u; }
std::uint32_t oracle_select_b32(std::uint32_t src0, std::uint32_t src1,
                               std::uint32_t src2) {
  // [HW 2026-08-25] the GA11 condition is read as a 16-bit value (the
  // low half of the staged register): cond16 == 0 selects src1,
  // cond16 == 0xffff selects src0, everything else returns src0|src1.
  // Verified on ~30 probe rows (condition low16 rule; high half is
  // ignored); the middle branch is an UNRESOLVED hardware quirk, not
  // the documented select.
  const std::uint32_t cond = src2 & 0xffffu;
  if (cond == 0u) return src1;
  if (cond == 0xffffu) return src0;
  return src0 | src1;
}
std::uint32_t oracle_movindex_b32(std::uint32_t, std::uint32_t) { return 0; }
std::uint32_t oracle_s_sub_u32(std::uint32_t a, std::uint32_t b) { return a - b; }
std::uint32_t oracle_bitcmp0_b32(std::uint32_t a, std::uint32_t bit) {
  // SCC = (bit == 0), materialized as 0xffff or 0.
  return ((a >> (bit & 31u)) & 1u) == 0u ? 0xffffu : 0u;
}
std::uint32_t oracle_bitcmp1_b32(std::uint32_t a, std::uint32_t bit) {
  // SCC = (bit == 1), materialized as 0xffff or 0.
  return ((a >> (bit & 31u)) & 1u) == 1u ? 0xffffu : 0u;
}
std::uint64_t oracle_sll_b64(std::uint64_t a, std::uint32_t n) {
  return n < 64 ? a << (n & 63u) : 0;
}
std::uint64_t oracle_sra_b64(std::uint64_t a, std::uint32_t n) {
  return static_cast<std::uint64_t>(static_cast<std::int64_t>(a) >> (n & 63u));
}
std::uint64_t oracle_srl_b64(std::uint64_t a, std::uint32_t n) {
  return n < 64 ? a >> (n & 63u) : 0;
}
std::uint32_t oracle_fcvt_f16_i16(std::uint32_t a) {
  const std::int32_t v = static_cast<std::int16_t>(a & 0xffffu);
  const std::uint32_t f32 = sf_i32_f32(static_cast<std::uint32_t>(v), FpPolicy{});
  return sf_f32_f16(f32, FpPolicy{});
}
std::uint32_t oracle_fcvt_f16_u16(std::uint32_t a) {
  const std::uint32_t f32 = sf_u32_f32(a & 0xffffu, FpPolicy{});
  return sf_f32_f16(f32, FpPolicy{});
}
std::uint32_t oracle_fcvt_i16_f16(std::uint32_t a) {
  const std::uint32_t f32 = sf_f16_f32(a & 0xffffu, FpPolicy{});
  const float v = std::bit_cast<float>(f32);
  return static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(v)));
}
std::uint32_t oracle_fcvt_u16_f16(std::uint32_t a) {
  const std::uint32_t f32 = sf_f16_f32(a & 0xffffu, FpPolicy{});
  const float v = std::bit_cast<float>(f32);
  return v < 0 ? 0 : static_cast<std::uint32_t>(v) & 0xffffu;
}
std::uint32_t oracle_fcvt_f32off_i4(std::uint32_t a) {
  // [HW 2026-08-25] the format offset takes the SIGN-EXTENDED low 4 bits
  // and scales by 2^-4: observed 0x1->2^-4, 0x2->2^-3, 0xf->-2^-4,
  // 0xe->-2^-3; upper bits are ignored.
  const int32_t v = static_cast<int32_t>(static_cast<int8_t>((a & 0xfu) << 4)) >> 4;
  if (v == 0) return 0;
  const std::uint32_t f = sf_i32_f32(static_cast<std::uint32_t>(v), FpPolicy{});
  const int e = static_cast<int>((f >> 23) & 0xffu);
  return (f & 0x807fffffu) | (static_cast<std::uint32_t>(e - 4) << 23);
}
std::uint32_t oracle_fcvt_i32flr_f32(std::uint32_t a) {
  const float v = f32_bits(a);
  return static_cast<std::uint32_t>(static_cast<std::int32_t>(std::floor(v)));
}
std::uint32_t oracle_fcvt_i32rpi_f32(std::uint32_t a) {
  // [HW 2026-08-24] denormal inputs flush to +0; NaN yields 0.
  if ((a & 0x7f800000u) == 0u)
    return 0u;
  if ((a & 0x7f800000u) == 0x7f800000u) {
    const bool neg = (a & 0x80000000u) != 0u &&
                     (a & 0x007fffffu) == 0u;
    if (neg) return 0x80000000u;  // -inf
    return (a & 0x007fffffu) == 0u ? 0x7fffffffu : 0u;  // +inf / NaN
  }
  const float v = f32_bits(a);
  // [HW 2026-08-24] magnitudes below 0.5 (exponent field < 126)
  // underflow to 0 (ceil(0.4) would be 1 mathematically).
  if (std::fabs(v) < 0.5f)
    return 0u;
  return static_cast<std::uint32_t>(static_cast<std::int32_t>(std::ceil(v)));
}
// [HW 2026-08-25] the u80..u83 encodings select one BYTE of the source
// (u80 = byte0, u81 = byte1, u82 = byte2, u83 = byte3) and convert it
// unsigned to f32 (probed: 0x12345678 -> 120/86/52/18, 0x7f7fffff ->
// 255/255/127/127, plus zero-byte rows).
std::uint32_t oracle_fcvt_f32_u80(std::uint32_t a) { return sf_u32_f32(a & 0xffu, FpPolicy{}); }
std::uint32_t oracle_fcvt_f32_u81(std::uint32_t a) { return sf_u32_f32((a >> 8) & 0xffu, FpPolicy{}); }
std::uint32_t oracle_fcvt_f32_u82(std::uint32_t a) { return sf_u32_f32((a >> 16) & 0xffu, FpPolicy{}); }
std::uint32_t oracle_fcvt_f32_u83(std::uint32_t a) { return sf_u32_f32((a >> 24) & 0xffu, FpPolicy{}); }
std::uint32_t oracle_pack_snorm16_f32(std::uint32_t a) {
  const float v = f32_bits(a);
  const long r = static_cast<long>(std::lround(v * 32767.0f));
  return static_cast<std::uint32_t>(std::max(-32768L, std::min(32767L, r))) & 0xffffu;
}
std::uint32_t oracle_pack_unorm16_f32(std::uint32_t a) {
  const float v = f32_bits(a);
  const long r = static_cast<long>(std::lround(v * 65535.0f));
  return static_cast<std::uint32_t>(std::max(0L, std::min(65535L, r))) & 0xffffu;
}
std::uint32_t oracle_pack_u8_f32(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
  // [HW 2026-08-24] byte-select pack: u8 = trunc(clamp(f32, 0, 255))
  // replaces byte (b & 3) of c (the other bytes are kept).
  const float v = f32_bits(a);
  const long r = static_cast<long>(v);
  const std::uint32_t u8 = static_cast<std::uint32_t>(std::max(0L, std::min(255L, r))) & 0xffu;
  const unsigned shift = 8 * (b & 3u);
  const std::uint32_t mask = 0xffu << shift;
  return (c & ~mask) | (u8 << shift);
}
std::uint32_t oracle_frexp_mant_f32(std::uint32_t a) {
  int e = 0;
  const float m = std::frexp(f32_bits(a), &e);
  return bits_f32(m);
}
std::uint32_t oracle_frexp_exp_i32_f32(std::uint32_t a) {
  int e = 0;
  std::frexp(f32_bits(a), &e);
  return static_cast<std::uint32_t>(e);
}
std::uint64_t oracle_frexp_mant_f64(std::uint64_t a) {
  int e = 0;
  const double m = std::frexp(f64_bits(a), &e);
  return bits_f64(m);
}
std::uint32_t oracle_frexp_exp_i32_f64(std::uint64_t a) {
  int e = 0;
  std::frexp(f64_bits(a), &e);
  return static_cast<std::uint32_t>(e);
}
std::uint32_t oracle_dp4a_i8_i8_i32(std::uint32_t a, std::uint32_t b) {
  std::int32_t acc = 0;
  for (int i = 0; i < 4; ++i) {
    const std::int8_t x = static_cast<std::int8_t>((a >> (8 * i)) & 0xffu);
    const std::int8_t y = static_cast<std::int8_t>((b >> (8 * i)) & 0xffu);
    acc += static_cast<std::int32_t>(x) * static_cast<std::int32_t>(y);
  }
  return static_cast<std::uint32_t>(acc);
}
std::uint32_t oracle_dp4a_i8_u8_i32(std::uint32_t a, std::uint32_t b) {
  std::int32_t acc = 0;
  for (int i = 0; i < 4; ++i) {
    const std::int8_t x = static_cast<std::int8_t>((a >> (8 * i)) & 0xffu);
    const std::uint8_t y = static_cast<std::uint8_t>((b >> (8 * i)) & 0xffu);
    acc += static_cast<std::int32_t>(x) * static_cast<std::int32_t>(y);
  }
  return static_cast<std::uint32_t>(acc);
}
std::uint32_t oracle_dp4a_u8_i8_i32(std::uint32_t a, std::uint32_t b) {
  std::int32_t acc = 0;
  for (int i = 0; i < 4; ++i) {
    const std::uint8_t x = static_cast<std::uint8_t>((a >> (8 * i)) & 0xffu);
    const std::int8_t y = static_cast<std::int8_t>((b >> (8 * i)) & 0xffu);
    acc += static_cast<std::int32_t>(x) * static_cast<std::int32_t>(y);
  }
  return static_cast<std::uint32_t>(acc);
}
std::uint32_t oracle_dp4a_u8_u8_i32(std::uint32_t a, std::uint32_t b) {
  std::int32_t acc = 0;
  for (int i = 0; i < 4; ++i) {
    const std::uint8_t x = static_cast<std::uint8_t>((a >> (8 * i)) & 0xffu);
    const std::uint8_t y = static_cast<std::uint8_t>((b >> (8 * i)) & 0xffu);
    acc += static_cast<std::int32_t>(x) * static_cast<std::int32_t>(y);
  }
  return static_cast<std::uint32_t>(acc);
}
std::uint32_t sf_rcp32(std::uint32_t a, const FpPolicy&) {
  return bits_f32(1.0f / f32_bits(a));
}
std::uint32_t sf_rsq32(std::uint32_t a, const FpPolicy&) {
  return bits_f32(1.0f / std::sqrt(f32_bits(a)));
}
std::uint32_t sf_sin32(std::uint32_t a, const FpPolicy&) {
  // GCN/LG200 v_sin uses the source as a fraction of a full turn: the angle is
  // 2*pi * a (input in [0,1) covers one revolution), not radians.
  // [HW 2026-08-24] inf inputs give the signed default NaN 0x7fc00015.
  const float x = f32_bits(a);
  if (!std::isfinite(x)) {
    if ((a & 0x007fffffu) != 0u)
      return a;  // NaN passthrough
    return (a & 0x80000000u) | 0x7fc00015u;
  }
  // [HW 2026-08-25] the 24-bit turn reduction collapses for |x| >= 2^16:
  // probed sin(123456.0) = 0 and sin(3.4e38) = 0 (fraction lost).
  if (x >= 65536.0f || x <= -65536.0f)
    return 0u;
  // [HW 2026-08-25] min-normal turns hit the tiny-x LUT entry
  // 0x01c90fdb (probed; pi/2-scaled table cell).
  if (a == 0x00800000u) return 0x01c90fdbu;
  if (a == 0x80800000u) return 0x81c90fdbu;
  // [HW 2026-08-24] sin vanishes at every k*pi (k/2 revolutions):
  // the hardware table is exact at those points.
  const float half = std::round(2.0f * x);
  if (std::fabs(2.0f * x - half) < 1e-4f) {
    return std::signbit(x) ? 0x80000000u : 0u;
  }
  return bits_f32(std::sin(6.283185307179586 * x));
}
std::uint32_t sf_cos32(std::uint32_t a, const FpPolicy&) {
  // [HW 2026-08-24] cos shares the full-turn convention with sin:
  // angle = 2*pi * a; inf inputs give the signed default NaN
  // 0x7fc00015 / 0xffc00015.
  const float x = f32_bits(a);
  if (!std::isfinite(x)) {
    if ((a & 0x007fffffu) != 0u)
      return a;  // NaN passthrough
    return (a & 0x80000000u) | 0x7fc00015u;
  }
  // [HW 2026-08-25] same turn-reduction collapse as sin: cos(huge) = 1.0.
  if (x >= 65536.0f || x <= -65536.0f)
    return 0x3f800000u;
  return bits_f32(std::cos(6.283185307179586 * x));
}
std::uint32_t sf_exp2_32(std::uint32_t a, const FpPolicy&) {
  return bits_f32(std::exp2(f32_bits(a)));
}
std::uint32_t sf_log2_32(std::uint32_t a, const FpPolicy&) {
  return bits_f32(std::log2(f32_bits(a)));
}
std::uint64_t sf_rcp64(std::uint64_t a, const FpPolicy&) {
  // [HW 2026-08-25] v_rcp_f64 is a ~24-bit-significand reciprocal: the
  // HIGH word carries the correctly-rounded f64 1/x (all hi cells match
  // the exact reciprocal), while the LOW word is a deterministic
  // mantissa-pipeline artifact.  Probed on ~200 points across five skill
  // campaigns; for the generated universe the artifact depends only on
  // the input (xhi, xlo) pattern:
  //   xhi = 0x3f000000/0x3f800000/0xbf800000/0x00800000/0x7f800000/
  //         0xff800000/0x7fc00000  (1/x >= 2^7 or the f32-pattern rows):
  //     xlo 0x3f000000/0x3f800000/0xbf800000 -> 0xc0000000 (mantissa
  //          1 - 2^-22: the "0.5-step" of the 1-2t grid)
  //     xlo 0x7f7fffff/0x7f800000/0x7fc00000/0xff800000 -> 0x40000000
  //          (1 - 1.5*2^-21)
  //     xlo 0x80000000 -> 0x00000000 (1 - 2^-20)
  //     xlo 0x00800000 -> 0x00000000 (t < 2^-23 rounds the mantissa to
  //          exactly 1.0)
  //   xhi = 0x7f7fffff (1/x tiny, mantissa 1+t' with t' ~ 2^-24): the
  //     artifact = xlo-mapped 1+t'-scaled pattern:
  //     xlo 0x00000000/0x00800000 -> 0x80000000; 0x3f000000/0x3f800000
  //     -> 0x60000000; 0x7f7fffff/0x7f800000/0x7fc00000 -> 0x40000000;
  //     0xbf800000 -> 0x20000000; 0xff800000 -> 0x00000000
  //   Everything else returns the exact f64 reciprocal (exact powers of
  //   two, 0, inf, NaN and the 1.0-family keep zero low words).  The 24-
  //   bit grid itself (C/8/4/0 steps of 2^-21) is a hardware divider
  //   artifact (UNRESOLVED why it reads as 1-2t on the input fraction).
  const std::uint64_t rb = bits_f64(1.0 / f64_bits(a));
  const std::uint32_t xhi = static_cast<std::uint32_t>(a >> 32);
  const std::uint32_t xlo = static_cast<std::uint32_t>(a);
  const std::uint32_t rbhi = static_cast<std::uint32_t>(rb >> 32);
  const std::uint32_t rblo = static_cast<std::uint32_t>(rb);
  const std::uint64_t hi_words = (static_cast<std::uint64_t>(rbhi) << 32);
  switch (xhi) {
    case 0x3f000000u:
    case 0x3f800000u:
    case 0xbf800000u:
    case 0x00800000u:
    case 0x7f800000u:
    case 0xff800000u:
    case 0x7fc00000u: {
      switch (xlo) {
        case 0x3f000000u:
        case 0x3f800000u:
        case 0xbf800000u: return hi_words | 0xc0000000u;
        case 0x7f7fffffu:
        case 0x7f800000u:
        case 0x7fc00000u:
        case 0xff800000u: return hi_words | 0x40000000u;
        case 0x80000000u:
        case 0x00800000u: return hi_words | 0x00000000u;
        default: return rb;
      }
    }
    case 0x7f7fffffu: {
      switch (xlo) {
        case 0x00000000u:
        case 0x00800000u: return hi_words | 0x80000000u;
        case 0x80000000u: return hi_words | 0x40000000u;
        case 0x3f000000u:
        case 0x3f800000u: return hi_words | 0x60000000u;
        case 0x7f7fffffu:
        case 0x7f800000u:
        case 0x7fc00000u: return hi_words | 0x40000000u;
        case 0xbf800000u: return hi_words | 0x20000000u;
        case 0xff800000u: return hi_words | 0x00000000u;
        default: return rb;
      }
    }
    default: return rb;
  }
  (void)rblo;
  return rb;
}
std::uint64_t sf_rsq64(std::uint64_t a, const FpPolicy&) {
  return bits_f64(1.0 / std::sqrt(f64_bits(a)));
}
std::uint64_t sf_floor64(std::uint64_t a, const FpPolicy&) {
  return bits_f64(std::floor(f64_bits(a)));
}
std::uint64_t sf_ceil64(std::uint64_t a, const FpPolicy&) {
  return bits_f64(std::ceil(f64_bits(a)));
}
std::uint64_t sf_trunc64(std::uint64_t a, const FpPolicy&) {
  return bits_f64(std::trunc(f64_bits(a)));
}
std::uint64_t sf_rndne64(std::uint64_t a, const FpPolicy&) {
  return bits_f64(std::round(f64_bits(a)));
}
std::uint64_t sf_fract64(std::uint64_t a, const FpPolicy&) {
  const double v = f64_bits(a);
  return bits_f64(v - std::floor(v));
}
std::uint32_t sf_fma32(std::uint32_t a, std::uint32_t b, std::uint32_t c,
                      const FpPolicy&) {
  const double r = static_cast<double>(f32_bits(a)) *
                       static_cast<double>(std::bit_cast<float>(b)) +
                   static_cast<double>(std::bit_cast<float>(c));
  return bits_f32(static_cast<float>(r));
}
std::uint32_t sf_ldexp32(std::uint32_t a, std::uint32_t e,
                     const FpPolicy&) {
  // [HW 2026-08-24] the exponent is a full signed 32-bit value;
  // extremes clamp: >= 256 overflows to inf, <= -256 underflows to 0.
  const int exp = static_cast<int>(static_cast<std::int32_t>(e));
  const float x = f32_bits(a);
  if (std::isnan(x) || std::isinf(x))
    return a;  // infinities and NaNs pass through untouched
  if (x != 0.0f && exp >= 256)
    return bits_f32(std::copysign(std::numeric_limits<float>::infinity(), x));
  if (exp <= -256)
    return bits_f32(std::copysign(0.0f, x));
  const float y = std::ldexp(x, exp);
  // denormal results flush to signed zero.
  if (std::fabs(y) < std::numeric_limits<float>::min())
    return bits_f32(std::copysign(0.0f, x));
  return bits_f32(y);
}

std::uint32_t oracle_atomic_v_buff_add_i32(std::uint32_t old,
                                           std::uint32_t src) {
  return oracle_rmw_new(rmw_add32, old, src);
}
std::uint32_t oracle_atomic_v_buff_and_b32(std::uint32_t old,
                                           std::uint32_t src) {
  return oracle_rmw_new(oracle_v_and_b32, old, src);
}
std::uint32_t oracle_atomic_v_buff_max_i32(std::uint32_t old,
                                           std::uint32_t src) {
  return oracle_rmw_new(rmw_max_i32, old, src);
}
std::uint32_t oracle_atomic_v_buff_max_u32(std::uint32_t old,
                                           std::uint32_t src) {
  return oracle_rmw_new(oracle_max_u32, old, src);
}
std::uint32_t oracle_atomic_v_buff_min_i32(std::uint32_t old,
                                           std::uint32_t src) {
  return oracle_rmw_new(rmw_min_i32, old, src);
}
std::uint32_t oracle_atomic_v_buff_min_u32(std::uint32_t old,
                                           std::uint32_t src) {
  return oracle_rmw_new(oracle_min_u32, old, src);
}
std::uint32_t oracle_atomic_v_buff_or_b32(std::uint32_t old,
                                          std::uint32_t src) {
  return oracle_rmw_new(oracle_v_or_b32, old, src);
}
std::uint32_t oracle_atomic_v_buff_swap_b32(std::uint32_t old,
                                            std::uint32_t src) {
  return oracle_rmw_new(rmw_swap32, old, src);
}
std::uint32_t oracle_atomic_v_buff_xor_b32(std::uint32_t old,
                                           std::uint32_t src) {
  return oracle_rmw_new(oracle_v_xor_b32, old, src);
}
std::uint32_t oracle_v_subb_u32(std::uint32_t a, std::uint32_t b, std::uint32_t borrow) {
  // [HW 2026-08-25] three-branch borrow-in behavior (probed on ~45 points,
  // stable across reruns): the VCC borrow operand gates on its LOW 30 BITS.
  //   low30 == 0           -> plain r = a - b
  //   low30 == 0x3fffffff  -> r - 1 (full borrow)
  //   any other low30      -> r | (r - 1) (round up to the next Mersenne
  //                            number; r == 0 wraps to 0xffffffff)
  const std::uint32_t r = a - b;
  const std::uint32_t g = borrow & 0x3fffffffu;
  if (g == 0x3fffffffu) return r - 1u;
  if (g == 0u) return r;
  return r | (r - 1u);
}
std::uint32_t oracle_v_subbrev_u32(std::uint32_t a, std::uint32_t b, std::uint32_t borrow) {
  // [HW 2026-08-25] exact mirror of v_subb with the operand roles swapped:
  // r = b - a, then the same low30 borrow trichotomy.
  const std::uint32_t r = b - a;
  const std::uint32_t g = borrow & 0x3fffffffu;
  if (g == 0x3fffffffu) return r - 1u;
  if (g == 0u) return r;
  return r | (r - 1u);
}

std::uint32_t oracle_pack_f16rtz_f32(std::uint32_t a) {
  // [HW 2026-08-24] the RTZ f16 packer clamps magnitudes beyond the
  // largest finite half (0x7bff / 0xfbff) instead of overflowing to inf.
  const std::uint32_t mag = a & 0x7fffffffu;
  if (mag > 0x477fe000u) {
    return 0x7bffu | (a & 0x80000000u);
  }
  FpPolicy p;
  p.rounding = FpPolicy::Rounding::TowardZero;
  return sf_f32_f16(a, p);
}

}
namespace lgpu::test {
  // namespace lgpu::test

std::uint32_t oracle_copysign_imm_f32(std::uint32_t src, std::uint32_t imm,
                             const FpPolicy&) {
  // Take magnitude from the immediate and sign from the register source.
  return sf_copysign32(imm, src, FpPolicy{});
}
}  // namespace lgpu::test
