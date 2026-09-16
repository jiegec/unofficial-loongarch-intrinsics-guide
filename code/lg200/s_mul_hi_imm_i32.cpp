// s_mul_hi_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_mul_hi_imm_i32, 2)
#include "s_mul_hi_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_mul_hi_imm_i32)
CASE(positive, 0x10000, 0x10000, 0x1)
CASE(negative, 0xffffffff, 0x80000000, 0x0)
CASE(max-square, 0x7fffffff, 0x7fffffff, 0x3fffffff)
LG200_CASE_END
