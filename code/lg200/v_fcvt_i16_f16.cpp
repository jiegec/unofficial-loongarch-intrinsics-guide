// v_fcvt_i16_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_i16_f16, 1)
#include "v_fcvt_i16_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_i16_f16)
CASE_FP(zero, rtne, 0x0, 0xa5a5a5a5)
CASE_FP(one, rtne, 0x3c00, 0xa5a5a5a5)
CASE_FP(neg, rtne, 0xbc00, 0xa5a5a5a5)
CASE_FP(nan-pos, rtne, 0x7e00, 0xa5a5a5a5)
CASE_FP(nan-neg, rtne, 0xfe00, 0xa5a5a5a5)
CASE_FP(inf-pos, rtne, 0x7c00, 0xa5a5a5a5)
CASE_FP(inf-neg, rtne, 0xfc00, 0xa5a5a5a5)
CASE_FP(zero-neg, rtne, 0x8000, 0xa5a5a5a5)
CASE_FP(subnormal-min, rtne, 0x1, 0xa5a5a5a5)
CASE_FP(subnormal-neg, rtne, 0x83ff, 0xa5a5a5a5)
CASE_FP(nan-signal-pos, rtne, 0x7c01, 0xa5a5a5a5)
CASE_FP(nan-signal-neg, rtne, 0xfc01, 0xa5a5a5a5)
CASE_FP(finite-max-pos, rtne, 0x7bff, 0xa5a5a5a5)
CASE_FP(finite-max-neg, rtne, 0xfbff, 0xa5a5a5a5)
CASE_FP(finite-min-pos, rtne, 0x400, 0xa5a5a5a5)
CASE_FP(finite-min-neg, rtne, 0x8400, 0xa5a5a5a5)
CASE_FP(i16-max-exact, rtne, 0x77ff, 0xa5a5a5a5)
CASE_FP(i16-neg-max-exact, rtne, 0xf7ff, 0xa5a5a5a5)
CASE_FP(i16-min-exact, rtne, 0xf800, 0xa5a5a5a5)
LG200_CASE_END
