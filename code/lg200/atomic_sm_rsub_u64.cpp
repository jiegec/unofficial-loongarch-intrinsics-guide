// atomic_sm_rsub_u64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_rsub_u64, 4)
#include "atomic_sm_rsub_u64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_sm_rsub_u64)
CASE(basic, 0x3, 0x0, 0xa, 0x0, 0x3, 0x0, 0x7, 0x0)
CASE(wrap, 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0xffffffff, 0xffffffff)
LG200_CASE_END
