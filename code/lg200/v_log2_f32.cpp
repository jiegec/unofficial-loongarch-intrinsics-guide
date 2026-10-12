// v_log2_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_log2_f32, 1)
#include "v_log2_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_log2_f32)
CASE_FP(one, rtne, 0x3f800000, 0x0)
CASE_FP(two, rtne, 0x40000000, 0x3f800000)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0xffc00000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x7f800000)
CASE_FP(inf-neg, rtne, 0xff800000, 0xffc00012)
CASE_FP(zero-neg, rtne, 0x80000000, 0xff800000)
CASE_FP(subnormal-min, rtne, 0x1, 0xc3150000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0xffc00012)
CASE_FP(zero-pos, rtne, 0x0, 0xff800000)
LG200_CASE_END
