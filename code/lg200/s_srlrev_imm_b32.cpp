// s_srlrev_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_srlrev_imm_b32, 2)
#include "s_srlrev_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_srlrev_imm_b32)
CASE(one, 0x1, 0x80000000, 0x40000000)
CASE(four, 0x4, 0x80000000, 0x8000000)
CASE(masked-count, 0x21, 0x80000000, 0x40000000)
LG200_CASE_END
