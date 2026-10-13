// v_fcvt_f64_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f64_f32, 1)
#include "v_fcvt_f64_f32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_fcvt_f64_f32)
CASE_FP(one, rtne, 0x3f800000, 0x0, 0x3ff00000)
CASE_FP(two, rtne, 0x40000000, 0x0, 0x40000000)
CASE_FP(neg, rtne, 0xbf800000, 0x0, 0xbff00000)
CASE_FP(subnormal-flush, rtne, 0x80000, 0x0, 0x0)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x0, 0x7ff80000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0x0, 0xfff80000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x0, 0x7ff00000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x0, 0xfff00000)
CASE_FP(zero-pos, rtne, 0x0, 0x0, 0x0)
CASE_FP(zero-neg, rtne, 0x80000000, 0x0, 0x80000000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x20000000, 0x7ff80000)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x20000000, 0xfff80000)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0xe0000000, 0x47efffff)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0xe0000000, 0xc7efffff)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x0, 0x38100000)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x0, 0xb8100000)
LG200_CASE_END
