// v_fcvt_i32_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_i32_f64, 2)
#include "v_fcvt_i32_f64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_i32_f64)
CASE_FP(integer, rtne, 0x0, 0x40450000, 0x2a)
CASE_FP(negative, rtne, 0x33333333, 0xc0457333, 0xffffffd6)
CASE_FP(saturate, rtne, 0x8800759c, 0x7e37e43c, 0x7fffffff)
CASE_FP(nan, rtne, 0x0, 0x7ff80000, 0x0)
CASE_FP(nan-pos, rtne, 0x0 0x7ff80000, 0x00000000)
CASE_FP(nan-neg, rtne, 0x0 0xfff80000, 0x00000000)
CASE_FP(inf-pos, rtne, 0x0 0x7ff00000, 0x7fffffff)
CASE_FP(inf-neg, rtne, 0x0 0xfff00000, 0x80000000)
CASE_FP(zero-pos, rtne, 0x0 0x0, 0x00000000)
CASE_FP(zero-neg, rtne, 0x0 0x80000000, 0x00000000)
CASE_FP(subnormal-min, rtne, 0x1 0x0, 0x00000000)
CASE_FP(subnormal-neg, rtne, 0xffffffff 0x800fffff, 0x00000000)
CASE_FP(nan-signal-pos, rtne, 0x1 0x7ff00000, 0x0)
CASE_FP(nan-signal-neg, rtne, 0x1 0xfff00000, 0x0)
CASE_FP(finite-max-pos, rtne, 0xffffffff 0x7fefffff, 0x7fffffff)
CASE_FP(finite-max-neg, rtne, 0xffffffff 0xffefffff, 0x80000000)
CASE_FP(finite-min-pos, rtne, 0x0 0x100000, 0x0)
CASE_FP(finite-min-neg, rtne, 0x0 0x80100000, 0x0)
LG200_CASE_END
