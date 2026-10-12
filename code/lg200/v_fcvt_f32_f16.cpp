// v_fcvt_f32_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32_f16, 1)
#include "v_fcvt_f32_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32_f16)
CASE_FP(one, rtne, 0x3c00, 0x3f800000)
CASE_FP(negative, rtne, 0xbc00, 0xbf800000)
CASE_FP(subnormal, rtne|daz, 0x1, 0x0)
CASE_FP(inf, rtne, 0x7c00, 0x7f800000)
CASE_FP(nan, rtne, 0x7e00, 0x7fc00000)
CASE_FP(zero-pos, rtne, 0x0, 0x00000000)
CASE_FP(zero-neg, rtne, 0x8000, 0x80000000)
CASE_FP(nan-signal-pos, rtne, 0x7c01, 0x7fc02000)
CASE_FP(nan-signal-neg, rtne, 0xfc01, 0xffc02000)
CASE_FP(finite-max-pos, rtne, 0x7bff, 0x477fe000)
CASE_FP(finite-max-neg, rtne, 0xfbff, 0xc77fe000)
CASE_FP(finite-min-pos, rtne, 0x400, 0x38800000)
CASE_FP(finite-min-neg, rtne, 0x8400, 0xb8800000)
LG200_CASE_END
