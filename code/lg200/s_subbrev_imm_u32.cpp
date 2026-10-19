// s_subbrev_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_subbrev_imm_u32, 3)
#include "s_subbrev_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_subbrev_imm_u32)
CASE(no-borrow, 0x2, 0x5, 0x0, 0x3)
CASE(borrow-in, 0x2, 0x5, 0x1, 0x2)
CASE(wrap, 0x5, 0x1, 0x0, 0xfffffffc)
LG200_CASE_END
