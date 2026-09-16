// s_sub_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_sub_imm_i32, 2)
#include "s_sub_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_sub_imm_i32)
CASE(basic, 0x5, 0x2, 0x3)
CASE(anchor, 0x13579bdf, 0x20000000, 0xf3579bdf)
CASE(min-wrap, 0x80000000, 0x1, 0x7fffffff)
LG200_CASE_END
