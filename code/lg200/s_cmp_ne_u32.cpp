// s_cmp_ne_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_cmp_ne_u32, 2)
#include "s_cmp_ne_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_cmp_ne_u32)
CASE(true, 0x13579bde, 0x13579bdf, 0xffff)
CASE(false, 0x13579bdf, 0x13579bdf, 0x0)
LG200_CASE_END
