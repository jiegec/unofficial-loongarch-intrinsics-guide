// atomic_sm_min_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_min_u32, 2)
#include "atomic_sm_min_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_sm_min_u32)
CASE(old-smaller, 0x3, 0x5, 0x3, 0x3)
CASE(operand-smaller, 0x5, 0x3, 0x5, 0x3)
CASE(zero, 0xffffffff, 0x0, 0xffffffff, 0x0)
LG200_CASE_END
