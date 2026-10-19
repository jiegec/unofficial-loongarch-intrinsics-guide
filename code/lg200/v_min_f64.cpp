// v_min_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_min_f64, 4)
#include "v_min_f64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_min_f64)
CASE_FP(basic, rtne, 0x0, 0x3ff00000, 0x0, 0x40000000, 0x0, 0x3ff00000)
CASE_FP(zero-signs, rtne, 0x0, 0x0, 0x0, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
