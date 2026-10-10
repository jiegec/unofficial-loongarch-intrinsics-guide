// v_bfrev_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bfrev_b32, 1)
#include "v_bfrev_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bfrev_b32)
CASE(zero, 0x0, 0x0)
CASE(one, 0x80000000, 0x1)
CASE(pattern, 0xffff0000, 0xffff)
CASE(all-ones, 0xffffffff, 0xffffffff)
LG200_CASE_END
