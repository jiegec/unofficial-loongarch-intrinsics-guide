// v_fcvt_f32_u80 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_u80, 1)
#include "v_fcvt_f32_u80.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_u80)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x1, 0x3f800000)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x00000000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0x00000000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x00000000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x00000000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x00000000)
LG200_CASE_END
