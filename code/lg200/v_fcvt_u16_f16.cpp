// v_fcvt_u16_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_u16_f16, 1)
#include "v_fcvt_u16_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_u16_f16)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x3c00, 0x1)
LG200_CASE_END
