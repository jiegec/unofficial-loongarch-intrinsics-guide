// v_cmp_imm_nlg_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_imm_nlg_f16, 2)
#include "v_cmp_imm_nlg_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_imm_nlg_f16)
CASE_FP(nan, rtne|daz|ftz, 0x7e00, 0x3c00, 0xffff)
CASE_FP(equal, rtne|daz|ftz, 0x3c00, 0x3c00, 0xffff)
CASE_FP(diff, rtne|daz|ftz, 0x3c00, 0x4000, 0x0)
LG200_CASE_END
