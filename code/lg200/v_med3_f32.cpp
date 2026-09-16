// v_med3_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_med3_f32, 3)
#include "v_med3_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_med3_f32)
CASE_FP(middle, rtne, 0x3f800000, 0x40000000, 0x40400000, 0x40000000)
CASE_FP(negative, rtne, 0xbf800000, 0x3f800000, 0x0, 0x0)
LG200_CASE_END
