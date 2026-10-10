// s_addc_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_addc_u32, 3)
#include "s_addc_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_addc_u32)
CASE(no-carry, 0x5, 0x2, 0x0, 0x7)
CASE(carry, 0x5, 0x2, 0x1, 0x8)
LG200_CASE_END
