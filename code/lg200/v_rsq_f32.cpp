// v_rsq_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_rsq_f32, 1)
#include "v_rsq_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_rsq_f32)
CASE_FP(four, rtne, 0x40800000, 0x3f000000)
CASE_FP(one, rtne, 0x3f800000, 0x3f800000)
LG200_CASE_END
