// v_fcvt_f16_u16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f16_u16, 1)
#include "v_fcvt_f16_u16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f16_u16)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x1, 0x3c00)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x00000000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0x00000000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x00000000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x00000000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x00000000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x3c00)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x3c00)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x7c00)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x7c00)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x0)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x0)
CASE_FP(u16-below-overflow, rtne, 0xffef, 0x7bff)
CASE_FP(u16-tie-at-overflow, rtne, 0xfff0, 0x7c00)
CASE_FP(u16-tie-even-down, rtne, 0x801, 0x6800)
LG200_CASE_END
