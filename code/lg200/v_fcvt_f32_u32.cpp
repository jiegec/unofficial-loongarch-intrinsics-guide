// v_fcvt_f32_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_u32, 1)
#include "v_fcvt_f32_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_u32)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x1, 0x3f800000)
CASE_FP(two, rtne, 0x2, 0x40000000)
CASE_FP(forty-two, rtne, 0x2a, 0x42280000)
CASE_FP(max, rtne, 0xffffffff, 0x4f800000)
CASE_FP(full-mantissa, rtne, 0x7fffff, 0x4afffffe)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x4eff0000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x4f7f8000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x4f000000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x4eff0000)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x4f7f8000)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x4eff0000)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x4f7f8000)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x4b000000)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x4f008000)
LG200_CASE_END
