// v_frexp_exp_i32_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_frexp_exp_i32_f32, 1)
#include "v_frexp_exp_i32_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_frexp_exp_i32_f32)
CASE_FP(one, rtne, 0x3f800000, 0x00000001)
CASE_FP(two, rtne, 0x40000000, 0x00000002)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x00000000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0x00000000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x00000000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x00000000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x00000000)
CASE_FP(subnormal-min, rtne, 0x1, 0x00000000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0x00000000)
CASE_FP(zero-pos, rtne, 0x0, 0x00000000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x00000000)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x00000000)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x00000080)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x00000080)
CASE_FP(finite-min-pos, rtne, 0x800000, 0xffffff83)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0xffffff83)
LG200_CASE_END
