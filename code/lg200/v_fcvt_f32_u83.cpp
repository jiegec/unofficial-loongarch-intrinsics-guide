// v_fcvt_f32_u83 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_u83, 1)
#include "v_fcvt_f32_u83.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_u83)
CASE_FP(zero, rtne, 0x0, 0x0)
LG200_CASE_END
