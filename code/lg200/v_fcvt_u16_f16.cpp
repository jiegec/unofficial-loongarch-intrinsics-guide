// v_fcvt_u16_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_u16_f16, 1)
#include "v_fcvt_u16_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_u16_f16)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(one, rtne, 0x3c00, 0x1)
CASE_FP(nan-pos, rtne, 0x7e00, 0x00000000)
CASE_FP(nan-neg, rtne, 0xfe00, 0x00000000)
CASE_FP(inf-pos, rtne, 0x7c00, 0x0000ffff)
CASE_FP(inf-neg, rtne, 0xfc00, 0x00000000)
CASE_FP(zero-neg, rtne, 0x8000, 0x00000000)
CASE_FP(subnormal-min, rtne, 0x1, 0x00000000)
CASE_FP(subnormal-neg, rtne, 0x83ff, 0x00000000)
LG200_CASE_END
