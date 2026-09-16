// s_subrev_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_subrev_imm_i32, 2)
#include "s_subrev_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_subrev_imm_i32)
CASE(basic, 0x2, 0x5, 0x3)
CASE(negative, 0x5, 0x1, 0xfffffffc)
CASE(anchor, 0x1, 0x13579bdf, 0x13579bde)
LG200_CASE_END
