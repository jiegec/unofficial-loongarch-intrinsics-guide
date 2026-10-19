// v_fcvt_f32_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_f64, 2)
#include "v_fcvt_f32_f64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_f64)
CASE_FP(one, rtne, 0x0, 0x3ff00000, 0x3f800000)
CASE_FP(underflow, rtne, 0x0, 0x37d00000, 0x0)
CASE_FP(negative, rtne, 0x0, 0xbff00000, 0xbf800000)
LG200_CASE_END
