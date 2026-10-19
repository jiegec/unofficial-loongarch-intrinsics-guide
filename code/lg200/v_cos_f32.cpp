// v_cos_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cos_f32, 1)
#include "v_cos_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cos_f32)
CASE_FP(zero, rtne, 0x0, 0x3f800000)
LG200_CASE_END
