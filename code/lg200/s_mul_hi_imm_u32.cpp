// s_mul_hi_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_mul_hi_imm_u32, 2)
#include "s_mul_hi_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_mul_hi_imm_u32)
CASE(small, 0x3, 0x4, 0x0)
CASE(max-square, 0xffffffff, 0xffffffff, 0xfffffffe)
CASE(anchor, 0x13579bdf, 0x10000, 0x1357)
LG200_CASE_END
