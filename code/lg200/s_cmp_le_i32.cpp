// s_cmp_le_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_cmp_le_i32, 2)
#include "s_cmp_le_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_cmp_le_i32)
CASE(equal, 0x13579bdf, 0x13579bdf, 0xffff)
CASE(negative, 0xfffffffe, 0xffffffff, 0xffff)
CASE(false, 0x1, 0xffffffff, 0x0)
LG200_CASE_END
