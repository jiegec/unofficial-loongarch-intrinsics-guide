// s_ctz_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_ctz_b32, 1)
#include "s_ctz_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_ctz_b32)
CASE(zero, 0x0, 0x20)
CASE(even, 0x80000000, 0x1f)
LG200_CASE_END
