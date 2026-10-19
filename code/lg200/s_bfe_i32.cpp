// s_bfe_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_bfe_i32, 3)
#include "s_bfe_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_bfe_i32)
CASE(basic, 0xff, 0x4, 0x4, 0xffffffff)
CASE(neg, 0xffffffff, 0x0, 0x8, 0xffffffff)
LG200_CASE_END
