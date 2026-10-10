// v_min3_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_min3_u32, 3)
#include "v_min3_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_min3_u32)
CASE(ordered, 0x1, 0x2, 0x3, 0x1)
CASE(reversed, 0x3, 0x2, 0x1, 0x1)
CASE(with-max, 0xffffffff, 0x80000000, 0x0, 0x0)
LG200_CASE_END
