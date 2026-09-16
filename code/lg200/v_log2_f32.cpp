// v_log2_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_log2_f32, 1)
#include "v_log2_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_log2_f32)
CASE_FP(one, rtne, 0x3f800000, 0x0)
CASE_FP(two, rtne, 0x40000000, 0x3f800000)
LG200_CASE_END
