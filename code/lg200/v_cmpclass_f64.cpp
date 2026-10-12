// v_cmpclass_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmpclass_f64, 4)
#include "v_cmpclass_f64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_cmpclass_f64)
CASE_FP(pos-normal, rtne|daz|ftz, 0x0, 0x3ff00000, 0x100, 0x0, 0xffff, 0x0)
CASE_FP(qnan, rtne|daz|ftz, 0x0, 0x7ff80000, 0x2, 0x0, 0xffff, 0x0)
CASE_FP(neg-inf, rtne|daz|ftz, 0x0, 0xfff00000, 0x4, 0x0, 0xffff, 0x0)
LG200_CASE_END
