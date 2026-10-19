// v_ctz_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_ctz_u32, 1)
#include "v_ctz_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_ctz_u32)
CASE(zero, 0x0, 0x20)
CASE(one, 0x1, 0x0)
CASE(even, 0x80000000, 0x1f)
CASE(mask, 0xff0000, 0x10)
LG200_CASE_END
