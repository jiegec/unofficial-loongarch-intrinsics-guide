// v_cmp_le_u64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_le_u64, 4)
#include "v_cmp_le_u64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_le_u64)
CASE(equal, 0x1, 0x0, 0x1, 0x0, 0xffff)
CASE(high, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff)
CASE(over, 0xffffffff, 0xffffffff, 0x1, 0x0, 0x0)
LG200_CASE_END
