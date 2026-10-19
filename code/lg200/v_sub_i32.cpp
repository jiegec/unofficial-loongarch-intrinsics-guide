// v_sub_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sub_i32, 2)
#include "v_sub_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sub_i32)
CASE(pos-pos, 0x3, 0x1, 0x2)
CASE(zero-minus-one, 0x0, 0x1, 0xffffffff)
CASE(neg-neg, 0xffffffff, 0xfffffffe, 0x1)
CASE(min-minus-one, 0x80000000, 0x1, 0x7fffffff)
CASE(max-minus-min, 0x7fffffff, 0x80000000, 0xffffffff)
LG200_CASE_END
