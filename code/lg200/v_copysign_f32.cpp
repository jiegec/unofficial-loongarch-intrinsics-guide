// v_copysign_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_copysign_f32, 2)
#include "v_copysign_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_copysign_f32)
CASE_FP(positive, rtne, 0x40600000, 0xbf800000, 0xc0600000)
CASE_FP(negative, rtne, 0xc0600000, 0x3f800000, 0x40600000)
CASE_FP(zero, rtne, 0x80000000, 0x3f800000, 0x0)
LG200_CASE_END
