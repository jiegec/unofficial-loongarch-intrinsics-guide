// v_add_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_add_imm_i32, 2)
#include "v_add_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_add_imm_i32)
CASE(basic, 0x5, 0x2, 0x7)
CASE(neg, 0xffffffff, 0x1, 0x0)
CASE(wrap, 0x7fffffff, 0x1, 0x80000000)
LG200_CASE_END
