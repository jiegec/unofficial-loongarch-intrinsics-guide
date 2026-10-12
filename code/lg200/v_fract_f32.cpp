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
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0xffc00000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x00000000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x00000000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x00000000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0x3f800000)
CASE_FP(zero-pos, rtne, 0x0, 0x00000000)
LG200_CASE_END
