// s_and_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_and_imm_b32, 2)
#include "s_and_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_and_imm_b32)
CASE(identity, 0x13579bdf, 0xffffffff, 0x13579bdf)
CASE(anchor, 0x13579bdf, 0xf0f0f0f, 0x3070b0f)
CASE(zero, 0x13579bdf, 0x0, 0x0)
LG200_CASE_END
