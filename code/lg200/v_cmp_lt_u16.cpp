// v_cmp_lt_u16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_lt_u16, 2)
#include "v_cmp_lt_u16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_lt_u16)
CASE(zero, 0x0, 0x1, 0xffff)
CASE(unsigned, 0x8000, 0xffff, 0xffff)
CASE(equal, 0x1, 0x1, 0x0)
LG200_CASE_END
