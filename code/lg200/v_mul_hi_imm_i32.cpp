// v_mul_hi_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul_hi_imm_i32, 2)
#include "v_mul_hi_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul_hi_imm_i32)
CASE(basic, 0x80000000, 0x80000000, 0x40000000)
CASE(zero, 0x0, 0x5, 0x0)
CASE(small, 0x10000, 0x10000, 0x1)
LG200_CASE_END
