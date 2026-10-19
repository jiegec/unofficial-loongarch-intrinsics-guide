// s_bfe_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_bfe_u32, 3)
#include "s_bfe_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_bfe_u32)
CASE(basic, 0xff, 0x4, 0x4, 0xf)
CASE(zero, 0x0, 0x0, 0x8, 0x0)
LG200_CASE_END
