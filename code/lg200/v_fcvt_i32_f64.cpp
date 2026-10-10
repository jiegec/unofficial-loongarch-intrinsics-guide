// v_fcvt_i32_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_i32_f64, 2)
#include "v_fcvt_i32_f64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_i32_f64)
CASE_FP(integer, rtne, 0x0, 0x40450000, 0x2a)
CASE_FP(negative, rtne, 0x33333333, 0xc0457333, 0xffffffd6)
CASE_FP(saturate, rtne, 0x8800759c, 0x7e37e43c, 0x7fffffff)
CASE_FP(nan, rtne, 0x0, 0x7ff80000, 0x0)
LG200_CASE_END
