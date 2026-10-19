// v_rsq_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_rsq_f64, 2)
#include "v_rsq_f64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_rsq_f64)
CASE_FP(four, rtne, 0x0, 0x40100000, 0x0, 0x3fe00000)
CASE_FP(one, rtne, 0x0, 0x3ff00000, 0x0, 0x3ff00000)
LG200_CASE_END
