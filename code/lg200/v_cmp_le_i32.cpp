// v_cmp_le_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_le_i32, 2)
#include "v_cmp_le_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_le_i32)
CASE(equal, 0x1, 0x1, 0xffff)
CASE(neg, 0xfffffffe, 0xffffffff, 0xffff)
CASE(pos, 0x2, 0x1, 0x0)
LG200_CASE_END
