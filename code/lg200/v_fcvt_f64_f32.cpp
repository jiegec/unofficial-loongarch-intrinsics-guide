// v_fcvt_f64_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f64_f32, 1)
#include "v_fcvt_f64_f32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_fcvt_f64_f32)
CASE_FP(one, rtne, 0x3f800000, 0x0, 0x3ff00000)
CASE_FP(two, rtne, 0x40000000, 0x0, 0x40000000)
CASE_FP(neg, rtne, 0xbf800000, 0x0, 0xbff00000)
CASE_FP(subnormal-flush, rtne, 0x80000, 0x0, 0x0)
LG200_CASE_END
