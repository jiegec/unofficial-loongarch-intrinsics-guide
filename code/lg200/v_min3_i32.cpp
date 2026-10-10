// v_min3_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_min3_i32, 3)
#include "v_min3_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_min3_i32)
CASE(negative, 0xffffffff, 0xfffffffe, 0x1, 0xfffffffe)
CASE(min-value, 0x80000000, 0x7fffffff, 0x0, 0x80000000)
LG200_CASE_END
