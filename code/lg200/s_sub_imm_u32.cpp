// s_sub_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_sub_imm_u32, 2)
#include "s_sub_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_sub_imm_u32)
CASE(basic, 0x5, 0x2, 0x3)
CASE(anchor, 0x13579bdf, 0x1, 0x13579bde)
CASE(wrap, 0x1, 0x5, 0xfffffffc)
LG200_CASE_END
