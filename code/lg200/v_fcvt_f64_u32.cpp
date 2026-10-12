// v_fcvt_f64_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f64_u32, 1)
#include "v_fcvt_f64_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_fcvt_f64_u32)
CASE_FP(zero, rtne, 0x0, 0x0, 0x0)
CASE_FP(max, rtne, 0xffffffff, 0xffe00000, 0x41efffff)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x0 0x41dfe000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x0 0x41eff000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x0 0x41e00000)
CASE_FP(subnormal-min, rtne, 0x1, 0x0 0x3ff00000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0xffe00000 0x41e00fff)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x400000 0x41dfe000)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x200000 0x41eff000)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0xffc00000 0x41dfdfff)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0xffe00000 0x41efefff)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x0 0x41600000)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x0 0x41e01000)
LG200_CASE_END
