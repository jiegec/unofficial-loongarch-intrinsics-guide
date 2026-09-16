// atomic_sm_add_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_add_u32, 2)
#include "atomic_sm_add_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_sm_add_u32)
CASE(add, 0x5, 0x3, 0x5, 0x8)
CASE(wrap, 0xffffffff, 0x1, 0xffffffff, 0x0)
CASE(zero, 0x0, 0x7, 0x0, 0x7)
LG200_CASE_END
