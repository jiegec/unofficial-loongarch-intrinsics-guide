// s_mul_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_mul_i32, 2)
#include "s_mul_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_mul_i32)
CASE(negative, 0xfffffffe, 0x3, 0xfffffffa)
CASE(small, 0x3, 0x4, 0xc)
LG200_CASE_END
