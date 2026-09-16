// v_floor_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_floor_f32, 1)
#include "v_floor_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_floor_f32)
CASE_FP(integer, rtne, 0x40400000, 0x40400000)
CASE_FP(positive, rtne, 0x40300000, 0x40000000)
CASE_FP(negative, rtne, 0xc0300000, 0xc0400000)
CASE_FP(frac-zero, rtne, 0x3e800000, 0x0)
CASE_FP(frac-neg, rtne, 0xbe800000, 0xbf800000)
CASE_FP(inf, rtne, 0x7f800000, 0x7f800000)
LG200_CASE_END
