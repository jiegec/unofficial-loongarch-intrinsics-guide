// s_subb_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_subb_imm_u32, 3)
#include "s_subb_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_subb_imm_u32)
CASE(no-borrow, 0x13579bdf, 0x1, 0x0, 0x13579bde)
CASE(borrow-in, 0x13579bdf, 0x1, 0x1, 0x13579bdd)
CASE(wrap, 0x0, 0x1, 0x0, 0xffffffff)
LG200_CASE_END
