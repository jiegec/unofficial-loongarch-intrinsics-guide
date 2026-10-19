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
CASE_FP(subnormal-product, rtne, 0x3800000, 0x35800000, 0x0)
CASE_FP(underflow, rtne|ftz, 0x3800000, 0x35800000, 0x0)
CASE_FP(overflow, rtne, 0x71c00000, 0x71c00000, 0x7f800000)
CASE_FP(overflow-clamp, rtne, 0x71c00000, 0x71c00000, 0x7f800000)
CASE_FP(zero-times-inf, rtne, 0x0, 0x7f800000, 0x7fc00000)
LG200_CASE_END
