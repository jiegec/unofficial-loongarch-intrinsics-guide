// v_fcvt_f32_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_i32, 1)
#include "v_fcvt_f32_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_i32)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x1, 0x3f800000)
CASE_FP(negative, rtne, 0xffffffff, 0xbf800000)
CASE_FP(max, rtne, 0x7fffffff, 0x4f000000)
CASE_FP(min, rtne, 0x80000000, 0xcf000000)
CASE_FP(forty-two, rtne, 0x2a, 0x42280000)
LG200_CASE_END
