// v_fract_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fract_f32, 1)
#include "v_fract_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fract_f32)
CASE_FP(positive, rtne, 0x40300000, 0x3f400000)
CASE_FP(negative, rtne, 0xc0300000, 0x3e800000)
CASE_FP(integer, rtne, 0x40400000, 0x0)
CASE_FP(tiny, rtne, 0x33000000, 0x33000000)
CASE_FP(tiny-negative, rtne, 0xb3000000, 0x3f800000)
LG200_CASE_END
