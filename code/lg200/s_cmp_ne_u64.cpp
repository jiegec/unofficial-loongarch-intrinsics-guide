// s_cmp_ne_u64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_cmp_ne_u64, 4)
#include "s_cmp_ne_u64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_cmp_ne_u64)
CASE(true, 0xffffffff, 0x0, 0x0, 0xffffffff, 0xffff)
CASE(false, 0x0, 0x1, 0x0, 0x1, 0x0)
CASE(zero, 0x0, 0x0, 0x0, 0x0, 0x0)
LG200_CASE_END
