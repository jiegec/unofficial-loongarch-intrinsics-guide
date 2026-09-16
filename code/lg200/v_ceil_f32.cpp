// v_ceil_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_ceil_f32, 1)
#include "v_ceil_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_ceil_f32)
CASE_FP(integer, rtne, 0x40400000, 0x40400000)
CASE_FP(positive, rtne, 0x40100000, 0x40400000)
CASE_FP(negative, rtne, 0xc0300000, 0xc0000000)
CASE_FP(frac-zero, rtne, 0xbe800000, 0x80000000)
CASE_FP(frac-pos, rtne, 0x3e800000, 0x3f800000)
LG200_CASE_END
