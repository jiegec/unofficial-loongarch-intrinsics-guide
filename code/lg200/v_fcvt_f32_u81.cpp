// v_fcvt_f32_u81 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_u81, 1)
#include "v_fcvt_f32_u81.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_u81)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(lsb-only, rtne, 0x1, 0x0)
CASE_FP(byte1-one, rtne, 0x100, 0x3f800000)
CASE_FP(byte1-255, rtne, 0xff00, 0x437f0000)
CASE_FP(all-ones, rtne, 0xffffffff, 0x437f0000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x00000000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x00000000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x00000000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x0)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x0)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x437f0000)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x437f0000)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x0)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x0)
LG200_CASE_END
