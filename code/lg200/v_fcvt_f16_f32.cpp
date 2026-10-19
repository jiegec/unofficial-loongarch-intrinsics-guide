// v_fcvt_f16_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f16_f32, 1)
#include "v_fcvt_f16_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f16_f32)
CASE_FP(one, rtne, 0x3f800000, 0x3c00)
CASE_FP(two, rtne, 0x40000000, 0x4000)
CASE_FP(half, rtne, 0x3f000000, 0x3800)
CASE_FP(negative, rtne, 0xbf800000, 0xbc00)
CASE_FP(subnormal, rtne, 0x33800000, 0x1)
CASE_FP(round-nearest, rtne, 0x3c8efa35, 0x2478)
CASE_FP(overflow, rtne, 0x71a202d0, 0x7c00)
LG200_CASE_END
