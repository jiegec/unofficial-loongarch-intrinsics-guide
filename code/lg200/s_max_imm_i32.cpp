// s_max_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_max_imm_i32, 2)
#include "s_max_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_max_imm_i32)
CASE(anchor-wins, 0x13579bdf, 0x80000000, 0x13579bdf)
CASE(imm-wins, 0x13579bdf, 0x7fffffff, 0x7fffffff)
CASE(negative, 0x80000000, 0xffffffff, 0xffffffff)
LG200_CASE_END
