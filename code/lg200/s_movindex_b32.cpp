// s_movindex_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_movindex_b32, 3)
#include "s_movindex_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_movindex_b32)
CASE(data, 0x12345678, 0x0, 0x0, 0x12345678)
LG200_CASE_END
