// v_max_imm_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_max_imm_f32, 2)
#include "v_max_imm_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_max_imm_f32)
CASE_FP(basic, rtne, 0x40000000, 0x3f800000, 0x40000000)
CASE_FP(neg, rtne, 0xc0000000, 0x3f800000, 0x3f800000)
LG200_CASE_END
