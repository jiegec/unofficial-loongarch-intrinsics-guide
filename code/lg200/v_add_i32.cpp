// v_add_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_add_i32, 2)
#include "v_add_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_add_i32)
CASE(pos-pos, 0x1, 0x2, 0x3)
CASE(neg-neg, 0xffffffff, 0xffffffff, 0xfffffffe)
CASE(pos-neg, 0x1, 0xfffffffe, 0xffffffff)
CASE(max-overflow, 0x7fffffff, 0x1, 0x80000000)
CASE(min-overflow, 0x80000000, 0x80000000, 0x0)
CASE(min-max, 0x80000000, 0x7fffffff, 0xffffffff)
LG200_CASE_END
