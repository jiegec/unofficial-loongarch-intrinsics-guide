// s_min_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_min_imm_i32, 2)
#include "s_min_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_min_imm_i32)
CASE(anchor, 0x13579bdf, 0x13579be0, 0x13579bdf)
CASE(min-value, 0x80000000, 0x1, 0x80000000)
CASE(negative-imm, 0x1, 0xffffffff, 0xffffffff)
LG200_CASE_END
