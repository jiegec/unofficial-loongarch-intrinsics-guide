// s_max_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_max_imm_u32, 2)
#include "s_max_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_max_imm_u32)
CASE(anchor, 0x13579bdf, 0x13579bde, 0x13579bdf)
CASE(max, 0x0, 0xffffffff, 0xffffffff)
LG200_CASE_END
