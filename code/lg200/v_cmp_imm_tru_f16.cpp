// v_cmp_imm_tru_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_imm_tru_f16, 2)
#include "v_cmp_imm_tru_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_imm_tru_f16)
CASE_FP(any, rtne|daz|ftz, 0x3c00, 0x4000, 0xffff)
CASE_FP(nan, rtne|daz|ftz, 0x7e00, 0x7e00, 0xffff)
CASE_FP(inf-pos, rtne, 0x7c00 0x3c00, 0x0000ffff)
CASE_FP(inf-neg, rtne, 0xfc00 0x3c00, 0x0000ffff)
CASE_FP(zero-pos, rtne, 0x0 0x3c00, 0x0000ffff)
CASE_FP(zero-neg, rtne, 0x8000 0x3c00, 0x0000ffff)
CASE_FP(subnormal-min, rtne, 0x1 0x3c00, 0x0000ffff)
CASE_FP(subnormal-neg, rtne, 0x83ff 0x3c00, 0x0000ffff)
LG200_CASE_END
