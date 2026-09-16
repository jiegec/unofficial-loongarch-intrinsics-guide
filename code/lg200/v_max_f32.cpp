// v_max_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_max_f32, 2)
#include "v_max_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_max_f32)
CASE_FP(basic, rtne, 0x3f800000, 0x40000000, 0x40000000)
CASE_FP(negative, rtne, 0xc0000000, 0xbf800000, 0xbf800000)
CASE_FP(zero-signs, rtne, 0x0, 0x80000000, 0x0)
CASE_FP(nan-first, rtne, 0x7fc00000, 0x40000000, 0x7fc00000)
LG200_CASE_END
