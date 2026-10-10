// s_select_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_select_b32, 3)
#include "s_select_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_select_b32)
CASE(scc-zero, 0x12345678, 0xffffffff, 0x0, 0xffffffff)
CASE(scc-one, 0x12345678, 0xffffffff, 0x1, 0x12345678)
CASE(scc-nonzero, 0x80000000, 0x7fffffff, 0x2, 0x80000000)
LG200_CASE_END
