// s_mul_hi_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_mul_hi_i32, 2)
#include "s_mul_hi_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_mul_hi_i32)
CASE(negative, 0xffffffff, 0x80000000, 0x0)
CASE(small, 0x3, 0x4, 0x0)
LG200_CASE_END
