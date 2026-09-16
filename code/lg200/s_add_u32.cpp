// s_add_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_add_u32, 2)
#include "s_add_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_add_u32)
CASE(zero-zero, 0x0, 0x0, 0x0)
CASE(one-two, 0x1, 0x2, 0x3)
CASE(max-one, 0xffffffff, 0x1, 0x0)
CASE(max-max, 0xffffffff, 0xffffffff, 0xfffffffe)
LG200_CASE_END
