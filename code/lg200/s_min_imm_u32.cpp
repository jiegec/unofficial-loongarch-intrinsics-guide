// s_min_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_min_imm_u32, 2)
#include "s_min_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_min_imm_u32)
CASE(zero, 0x0, 0xffffffff, 0x0)
CASE(anchor, 0x13579bdf, 0x13579be0, 0x13579bdf)
LG200_CASE_END
