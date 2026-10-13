// v_fcvt_f64_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f64_i32, 1)
#include "v_fcvt_f64_i32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_fcvt_f64_i32)
CASE_FP(zero, rtne, 0x0, 0x0, 0x0)
CASE_FP(one, rtne, 0x1, 0x0, 0x3ff00000)
CASE_FP(negative, rtne, 0xffffffff, 0x0, 0xbff00000)
CASE_FP(max, rtne, 0x7fffffff, 0xffc00000, 0x41dfffff)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x0, 0x41dfe000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x0, 0xc1600000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x0, 0xc1e00000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x400000, 0x41dfe000)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0xc0000000, 0xc15fffff)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0xffc00000, 0x41dfdfff)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x20000000, 0xc1600000)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x0, 0x41600000)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x0, 0xc1dfe000)
LG200_CASE_END
