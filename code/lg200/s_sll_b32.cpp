// s_sll_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_sll_b32, 2)
#include "s_sll_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_sll_b32)
CASE(one, 0x1, 0x1, 0x2)
CASE(top-bit, 0x1, 0x1f, 0x80000000)
LG200_CASE_END
