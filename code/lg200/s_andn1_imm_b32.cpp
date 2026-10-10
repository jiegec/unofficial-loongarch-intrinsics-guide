// s_andn1_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_andn1_imm_b32, 2)
#include "s_andn1_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_andn1_imm_b32)
CASE(basic, 0xffffffff, 0xf0f0f0f, 0x0)
CASE(anchor, 0x13579bdf, 0xf0f0f0f, 0xc080400)
CASE(clear, 0xf0f0f0f, 0xf0f0f0f, 0x0)
LG200_CASE_END
