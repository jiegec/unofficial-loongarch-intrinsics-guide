// s_ineg_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_ineg_i32, 1)
#include "s_ineg_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_ineg_i32)
CASE(one, 0x1, 0xffffffff)
CASE(minus-one, 0xffffffff, 0x1)
LG200_CASE_END
