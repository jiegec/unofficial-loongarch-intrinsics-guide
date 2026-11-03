// v_fcvt_f16_i16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f16_i16, 1)
#include "v_fcvt_f16_i16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f16_i16)
CASE_FP(zero, rtne, 0x0, 0xa5a5a5a5)
CASE_FP(one, rtne, 0x1, 0xa5a5a5a5)
CASE_FP(neg, rtne, 0xffffffff, 0xa5a5a5a5)
CASE_FP(inf-pos, rtne, 0x7f800000, 0xa5a5a5a5)
CASE_FP(inf-neg, rtne, 0xff800000, 0xa5a5a5a5)
CASE_FP(zero-neg, rtne, 0x80000000, 0xa5a5a5a5)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0xa5a5a5a5)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0xa5a5a5a5)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0xa5a5a5a5)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0xa5a5a5a5)
CASE_FP(finite-min-pos, rtne, 0x800000, 0xa5a5a5a5)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0xa5a5a5a5)
CASE_FP(i16-tie-even-down, rtne, 0x801, 0xa5a5a5a5)
CASE_FP(i16-tie-even-up, rtne, 0x803, 0xa5a5a5a5)
CASE_FP(i16-largest-exact, rtne, 0x7fe0, 0xa5a5a5a5)
LG200_CASE_END
