// s_sub_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_sub_i32, 2)
#include "s_sub_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_sub_i32)
CASE(basic, 0x5, 0x2, 0x3)
CASE(neg, 0x0, 0x1, 0xffffffff)
LG200_CASE_END
