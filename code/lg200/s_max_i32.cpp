// s_max_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_max_i32, 2)
#include "s_max_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_max_i32)
CASE(negative, 0xffffffff, 0x1, 0x1)
CASE(basic, 0x5, 0xa, 0xa)
LG200_CASE_END
