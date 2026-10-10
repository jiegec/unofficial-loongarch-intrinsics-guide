// s_bfrev_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_bfrev_b32, 1)
#include "s_bfrev_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_bfrev_b32)
CASE(one, 0x80000000, 0x1)
CASE(pattern, 0xffff0000, 0xffff)
LG200_CASE_END
