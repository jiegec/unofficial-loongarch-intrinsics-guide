// v_cmp_lt_u64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_lt_u64, 4)
#include "v_cmp_lt_u64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_lt_u64)
CASE(zero, 0x0, 0x0, 0x1, 0x0, 0xffff)
CASE(unsigned, 0x0, 0x80000000, 0xffffffff, 0xffffffff, 0xffff)
CASE(equal, 0x1, 0x0, 0x1, 0x0, 0x0)
LG200_CASE_END
