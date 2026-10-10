// s_orn2_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_orn2_imm_b32, 2)
#include "s_orn2_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_orn2_imm_b32)
CASE(all-ones, 0xf0f0f0f, 0xf0f0f0f, 0xffffffff)
CASE(anchor, 0x13579bdf, 0xf0f0f0f, 0xf3f7fbff)
LG200_CASE_END
