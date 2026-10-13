// v_cmp_nge_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_nge_f16, 2)
#include "v_cmp_nge_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_nge_f16)
CASE_FP(nan, rtne|daz|ftz, 0x7e00, 0x3c00, 0xffff)
CASE_FP(lt, rtne|daz|ftz, 0x3c00, 0x4000, 0xffff)
CASE_FP(ge, rtne|daz|ftz, 0x4000, 0x4000, 0x0)
CASE_FP(inf-pos, rtne, 0x7c00, 0x3c00, 0x00000000)
CASE_FP(inf-neg, rtne, 0xfc00, 0x3c00, 0x0000ffff)
CASE_FP(zero-pos, rtne, 0x0, 0x3c00, 0x0000ffff)
CASE_FP(zero-neg, rtne, 0x8000, 0x3c00, 0x0000ffff)
CASE_FP(subnormal-min, rtne, 0x1, 0x3c00, 0x0000ffff)
CASE_FP(subnormal-neg, rtne, 0x83ff, 0x3c00, 0x0000ffff)
CASE_FP(nan-signal-pos, rtne, 0x7c01, 0x3c00, 0xffff)
CASE_FP(nan-signal-neg, rtne, 0xfc01, 0x3c00, 0xffff)
CASE_FP(finite-max-pos, rtne, 0x7bff, 0x3c00, 0x0)
CASE_FP(finite-max-neg, rtne, 0xfbff, 0x3c00, 0xffff)
CASE_FP(finite-min-pos, rtne, 0x400, 0x3c00, 0xffff)
CASE_FP(finite-min-neg, rtne, 0x8400, 0x3c00, 0xffff)
LG200_CASE_END
