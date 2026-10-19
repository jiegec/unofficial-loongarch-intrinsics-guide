// v_ldexp_imm_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_ldexp_imm_f32, 2)
#include "v_ldexp_imm_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_ldexp_imm_f32)
CASE_FP(scale-up, rtne, 0x3f800000, 0x2, 0x40800000)
CASE_FP(scale-down, rtne, 0x3f800000, 0xffffffff, 0x3f000000)
LG200_CASE_END
