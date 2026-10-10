// v_fcvt_i32_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_i32_f32, 1)
#include "v_fcvt_i32_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_i32_f32)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x3f800000, 0x1)
CASE_FP(minus-one, rtne, 0xbf800000, 0xffffffff)
CASE_FP(forty-two, rtne, 0x42280000, 0x2a)
CASE_FP(neg-frac, rtne, 0xc22b3333, 0xffffffd6)
CASE_FP(clamp-pos, rtne, 0x4f000000, 0x7fffffff)
CASE_FP(clamp-neg, rtne, 0xcf000000, 0x80000000)
CASE_FP(nan, rtne, 0x7fc00000, 0x0)
LG200_CASE_END
