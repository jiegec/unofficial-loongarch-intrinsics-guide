// s_abs_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_abs_i32, 1)
#include "s_abs_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_abs_i32)
CASE(minus-one, 0xffffffff, 0x1)
CASE(min-wraps, 0x80000000, 0x80000000)
LG200_CASE_END
