// v_fcvt_f64_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f64_i32, 1)
#include "v_fcvt_f64_i32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_fcvt_f64_i32)
CASE_FP(zero, rtne, 0x0, 0x0, 0x0)
CASE_FP(one, rtne, 0x1, 0x0, 0x3ff00000)
CASE_FP(negative, rtne, 0xffffffff, 0x0, 0xbff00000)
CASE_FP(max, rtne, 0x7fffffff, 0xffc00000, 0x41dfffff)
LG200_CASE_END
