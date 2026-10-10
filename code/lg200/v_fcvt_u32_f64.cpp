// v_fcvt_u32_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_u32_f64, 2)
#include "v_fcvt_u32_f64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_u32_f64)
CASE_FP(integer, rtne, 0x0, 0x40450000, 0x2a)
CASE_FP(negative, rtne, 0x0, 0xbff00000, 0x0)
CASE_FP(saturate, rtne, 0x8800759c, 0x7e37e43c, 0xffffffff)
LG200_CASE_END
