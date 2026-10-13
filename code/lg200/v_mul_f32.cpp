// v_mul_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul_f32, 2)
#include "v_mul_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul_f32)
CASE_FP(one-times-two, rtne, 0x3f800000, 0x40000000, 0x40000000)
CASE_FP(neg-times-neg, rtne, 0xc0000000, 0xc0400000, 0x40c00000)
// Hardware runs RN + denormal-flush always; overflow yields +inf (no clamp) and
// rounding/flush policy tokens are ignored by the fixed FP default.
CASE_FP(subnormal-product, rtne, 0x3800000, 0x35800000, 0x00000000)
CASE_FP(underflow, rtne|ftz, 0x3800000, 0x35800000, 0x00000000)
CASE_FP(overflow, rtne, 0x71c00000, 0x71c00000, 0x7f800000)
CASE_FP(overflow-clamp, rtne, 0x71c00000, 0x71c00000, 0x7f800000)
CASE_FP(zero-times-inf, rtne, 0x0, 0x7f800000, 0x7fc00000)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x3f800000, 0x7fc00000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0x3f800000, 0x7fc00000)
CASE_FP(subnormal-min, rtne, 0x1, 0x3f800000, 0x00000000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0x3f800000, 0x80000000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x3f800000, 0x80000000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x3f800000, 0x7fc00001)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x3f800000, 0xffc00001)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x3f800000, 0x7f7fffff)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x3f800000, 0xff7fffff)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x3f800000, 0x00800000)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x3f800000, 0x80800000)
CASE_FP(one-times-two, rtz, 0x3f800000, 0x40000000, 0x40000000)
CASE_FP(one-times-two, rtup, 0x3f800000, 0x40000000, 0x40000000)
CASE_FP(one-times-two, rtdn, 0x3f800000, 0x40000000, 0x40000000)
CASE_FP(neg-times-neg, rtz, 0xc0000000, 0xc0400000, 0x40c00000)
CASE_FP(neg-times-neg, rtup, 0xc0000000, 0xc0400000, 0x40c00000)
CASE_FP(neg-times-neg, rtdn, 0xc0000000, 0xc0400000, 0x40c00000)
CASE_FP(subnormal-product, rtz, 0x3800000, 0x35800000, 0x00000000)
CASE_FP(subnormal-product, rtup, 0x3800000, 0x35800000, 0x00000000)
CASE_FP(subnormal-product, rtdn, 0x3800000, 0x35800000, 0x00000000)
LG200_CASE_END
