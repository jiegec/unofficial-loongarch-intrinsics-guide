// v_mul_hi_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul_hi_imm_u32, 2)
#include "v_mul_hi_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul_hi_imm_u32)
CASE(zero, 0x0, 0x5, 0x0)
CASE(small, 0x10000, 0x10000, 0x1)
CASE(wrap, 0xffffffff, 0xffffffff, 0xfffffffe)
LG200_CASE_END
