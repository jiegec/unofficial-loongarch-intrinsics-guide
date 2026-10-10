// v_mul_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul_i32, 2)
#include "v_mul_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul_i32)
CASE(neg-neg, 0xffffffff, 0xffffffff, 0x1)
CASE(neg-pos, 0xfffffffe, 0x3, 0xfffffffa)
CASE(min-square, 0x80000000, 0x80000000, 0x0)
LG200_CASE_END
