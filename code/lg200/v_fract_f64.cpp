// v_fract_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fract_f64, 2)
#include "v_fract_f64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_fract_f64)
CASE_FP(pos, rtne, 0x0, 0x3ff80000, 0x0, 0x3fe00000)
CASE_FP(neg, rtne, 0x0, 0xbff80000, 0x0, 0x3fe00000)
LG200_CASE_END
