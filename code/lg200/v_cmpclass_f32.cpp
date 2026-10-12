// v_cmpclass_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmpclass_f32, 2)
#include "v_cmpclass_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmpclass_f32)
CASE_FP(pos-normal, rtne|daz|ftz, 0x3f800000, 0x100, 0xffff)
CASE_FP(qnan, rtne|daz|ftz, 0x7fc00000, 0x2, 0xffff)
CASE_FP(snan-only, rtne|daz|ftz, 0x7f800001, 0x2, 0x0)
CASE_FP(norm-mask, rtne|daz|ftz, 0x3f800000, 0x108, 0xffff)
CASE_FP(sub-mask, rtne|daz|ftz, 0x80000, 0x108, 0x0)
CASE_FP(finite-nan, rtne|daz|ftz, 0x7fc00000, 0x1f8, 0x0)
CASE_FP(finite-inf, rtne|daz|ftz, 0x7f800000, 0x1f8, 0x0)
CASE_FP(finite-norm, rtne|daz|ftz, 0x3f800000, 0x1f8, 0xffff)
LG200_CASE_END
