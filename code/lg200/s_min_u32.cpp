// s_min_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_min_u32, 2)
#include "s_min_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_min_u32)
CASE(basic, 0x5, 0xa, 0x5)
CASE(reverse, 0xa, 0x5, 0x5)
LG200_CASE_END
