// v_fcvt_f32_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_f16, 1)
#include "v_fcvt_f32_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_f16)
CASE_FP(one, rtne, 0x3c00, 0x3f800000)
CASE_FP(negative, rtne, 0xbc00, 0xbf800000)
CASE_FP(subnormal, rtne|daz, 0x1, 0x0)
CASE_FP(inf, rtne, 0x7c00, 0x7f800000)
CASE_FP(nan, rtne, 0x7e00, 0x7fc00000)
LG200_CASE_END
