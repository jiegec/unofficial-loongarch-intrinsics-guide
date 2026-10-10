// s_or_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_or_imm_b32, 2)
#include "s_or_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_or_imm_b32)
CASE(identity, 0x13579bdf, 0x0, 0x13579bdf)
CASE(anchor, 0x13579bdf, 0xf0f0f0f0, 0xf3f7fbff)
CASE(all, 0x13579bdf, 0xffffffff, 0xffffffff)
LG200_CASE_END
