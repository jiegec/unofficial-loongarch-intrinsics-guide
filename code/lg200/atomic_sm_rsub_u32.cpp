// atomic_sm_rsub_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_rsub_u32, 2)
#include "atomic_sm_rsub_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_sm_rsub_u32)
CASE(basic, 0x3, 0xa, 0x3, 0x7)
CASE(wrap, 0x1, 0x0, 0x1, 0xffffffff)
CASE(equal, 0x7, 0x7, 0x7, 0x0)
LG200_CASE_END
