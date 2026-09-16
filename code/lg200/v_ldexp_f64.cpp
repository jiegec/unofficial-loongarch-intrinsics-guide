// v_ldexp_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_ldexp_f64, 3)
#include "v_ldexp_f64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_ldexp_f64)
CASE_FP(scale-up, rtne, 0x0, 0x3ff00000, 0x2, 0x0, 0x40100000)
CASE_FP(scale-down, rtne, 0x0, 0x3ff00000, 0xfffffffe, 0x0, 0x3fd00000)
CASE_FP(overflow, rtne, 0x0, 0x3ff00000, 0x7d0, 0x0, 0x7ff00000)
LG200_CASE_END
