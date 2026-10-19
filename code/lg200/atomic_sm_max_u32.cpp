// atomic_sm_max_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_max_u32, 2)
#include "atomic_sm_max_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_sm_max_u32)
CASE(operand-larger, 0x5, 0x9, 0x5, 0x9)
CASE(old-larger, 0x9, 0x5, 0x9, 0x9)
CASE(max, 0x80000000, 0xffffffff, 0x80000000, 0xffffffff)
LG200_CASE_END
