// v_cmp_imm_lt_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_imm_lt_u32, 2)
#include "v_cmp_imm_lt_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_imm_lt_u32)
CASE(zero, 0x0, 0x1, 0xffff)
CASE(unsigned, 0x80000000, 0xffffffff, 0xffff)
CASE(equal, 0x1, 0x1, 0x0)
LG200_CASE_END
