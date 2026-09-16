// v_med3_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_med3_u32, 3)
#include "v_med3_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_med3_u32)
CASE(middle, 0x1, 0x2, 0x3, 0x2)
CASE(reversed, 0x3, 0x2, 0x1, 0x2)
CASE(extremes, 0x0, 0xffffffff, 0x7fffffff, 0x7fffffff)
LG200_CASE_END
