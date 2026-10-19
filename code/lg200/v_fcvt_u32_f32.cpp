// v_fcvt_u32_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_u32_f32, 1)
#include "v_fcvt_u32_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_u32_f32)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x3f800000, 0x1)
CASE_FP(two, rtne, 0x40000000, 0x2)
CASE_FP(forty-two, rtne, 0x42280000, 0x2a)
CASE_FP(neg, rtne, 0xbf800000, 0x0)
CASE_FP(clamp, rtne, 0x4f800000, 0xffffffff)
CASE_FP(frac, rtne, 0x422b3333, 0x2a)
LG200_CASE_END
