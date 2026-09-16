// s_sllrev_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_sllrev_imm_b32, 2)
#include "s_sllrev_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_sllrev_imm_b32)
CASE(one, 0x1, 0x1, 0x2)
CASE(four, 0x4, 0x1, 0x10)
CASE(masked-count, 0x21, 0x1, 0x2)
LG200_CASE_END
