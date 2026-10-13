// v_cmp_lg_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_lg_f32, 2)
#include "v_cmp_lg_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_lg_f32)
CASE_FP(diff, rtne|daz|ftz, 0x3f800000, 0x40000000, 0xffff)
CASE_FP(nan, rtne|daz|ftz, 0x7fc00000, 0x3f800000, 0x0)
CASE_FP(equal, rtne|daz|ftz, 0x3f800000, 0x3f800000, 0x0)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x3f800000, 0x0000ffff)
CASE_FP(inf-neg, rtne, 0xff800000, 0x3f800000, 0x0000ffff)
CASE_FP(zero-neg, rtne, 0x80000000, 0x3f800000, 0x0000ffff)
CASE_FP(subnormal-min, rtne, 0x1, 0x3f800000, 0x0000ffff)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0x3f800000, 0x0000ffff)
CASE_FP(zero-pos, rtne, 0x0, 0x3f800000, 0x0000ffff)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x3f800000, 0x0)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x3f800000, 0x0)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x3f800000, 0xffff)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x3f800000, 0xffff)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x3f800000, 0xffff)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x3f800000, 0xffff)
LG200_CASE_END
