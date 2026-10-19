// v_sub_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sub_f32, 2)
#include "v_sub_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sub_f32)
CASE_FP(one-minus-one, rtne, 0x3f800000, 0x3f800000, 0x0)
CASE_FP(zero-minus-one, rtne, 0x0, 0x3f800000, 0xbf800000)
// Hardware flushes subnormal OPERAND inputs to zero (DAZ): 0x00400000 (2^-127)
// is read as 0, so 2^-126 - 2^-127 yields 2^-126 (0x00800000), not the exact
// 0x00400000 result.
CASE_FP(subnormal, rtne, 0x800000, 0x400000, 0x800000)
CASE_FP(subnormal-ftz, rtne|ftz, 0x800000, 0x400000, 0x800000)
CASE_FP(inf-inf, rtne, 0x7f800000, 0x7f800000, 0x7fc00000)
LG200_CASE_END
