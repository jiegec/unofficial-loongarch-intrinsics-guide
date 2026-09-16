// v_fcvt_f64_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f64_u32, 1)
#include "v_fcvt_f64_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_fcvt_f64_u32)
CASE_FP(zero, rtne, 0x0, 0x0, 0x0)
CASE_FP(max, rtne, 0xffffffff, 0xffe00000, 0x41efffff)
LG200_CASE_END
