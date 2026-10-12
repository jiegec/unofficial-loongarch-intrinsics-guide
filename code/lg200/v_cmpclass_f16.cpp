// v_cmpclass_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmpclass_f16, 2)
#include "v_cmpclass_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmpclass_f16)
CASE_FP(pos-normal, rtne|daz|ftz, 0x3c00, 0x100, 0xffff)
CASE_FP(qnan, rtne|daz|ftz, 0x7e00, 0x2, 0xffff)
CASE_FP(pos-inf, rtne|daz|ftz, 0x7c00, 0x200, 0xffff)
CASE_FP(zero-pos, rtne, 0x0 0x3c00, 0x00000000)
CASE_FP(zero-neg, rtne, 0x8000 0x3c00, 0x00000000)
LG200_CASE_END
