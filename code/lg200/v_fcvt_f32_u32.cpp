// v_fcvt_f32_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_u32, 1)
#include "v_fcvt_f32_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_u32)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x1, 0x3f800000)
CASE_FP(two, rtne, 0x2, 0x40000000)
CASE_FP(forty-two, rtne, 0x2a, 0x42280000)
CASE_FP(max, rtne, 0xffffffff, 0x4f800000)
CASE_FP(full-mantissa, rtne, 0x7fffff, 0x4afffffe)
LG200_CASE_END
