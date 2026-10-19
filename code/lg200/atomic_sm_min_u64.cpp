// atomic_sm_min_u64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_min_u64, 4)
#include "atomic_sm_min_u64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_sm_min_u64)
CASE(old-smaller, 0x3, 0x0, 0x5, 0x0, 0x3, 0x0, 0x3, 0x0)
CASE(zero, 0xffffffff, 0xffffffff, 0x0, 0x0, 0xffffffff, 0xffffffff, 0x0, 0x0)
LG200_CASE_END
