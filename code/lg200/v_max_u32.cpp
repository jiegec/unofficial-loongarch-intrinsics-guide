// v_max_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_max_u32, 2)
#include "v_max_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_max_u32)
CASE(zero, 0x0, 0x1, 0x1)
CASE(equal, 0x7, 0x7, 0x7)
CASE(reversed, 0xffffffff, 0x80000000, 0xffffffff)
LG200_CASE_END
