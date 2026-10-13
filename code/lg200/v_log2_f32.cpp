// v_log2_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_log2_f32, 1)
#include "v_log2_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_log2_f32)
CASE_FP(one, rtne, 0x3f800000, 0x00000000)
CASE_FP(two, rtne, 0x40000000, 0x3f800000)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0xffc00000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x7f800000)
CASE_FP(inf-neg, rtne, 0xff800000, 0xffc00012)
CASE_FP(zero-neg, rtne, 0x80000000, 0xff800000)
CASE_FP(subnormal-min, rtne, 0x1, 0xff800000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0xff800000)
CASE_FP(zero-pos, rtne, 0x0, 0xff800000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x7fc00001)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0xffc00001)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x43000000)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0xffc00012)
CASE_FP(finite-min-pos, rtne, 0x800000, 0xc2fc0000)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0xffc00012)
CASE_FP(one, rtz, 0x3f800000, 0x00000000)
CASE_FP(one, rtup, 0x3f800000, 0x00000000)
CASE_FP(one, rtdn, 0x3f800000, 0x00000000)
CASE_FP(two, rtz, 0x40000000, 0x3f800000)
CASE_FP(two, rtup, 0x40000000, 0x3f800000)
CASE_FP(two, rtdn, 0x40000000, 0x3f800000)
CASE_FP(nan-pos, rtz, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-pos, rtup, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-pos, rtdn, 0x7fc00000, 0x7fc00000)
LG200_CASE_END
