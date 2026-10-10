// s_cmp_imm_ge_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_cmp_imm_ge_u32, 2)
#include "s_cmp_imm_ge_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_cmp_imm_ge_u32)
CASE(equal, 0xffffffff, 0xffffffff, 0xffff)
CASE(false, 0x80000000, 0xffffffff, 0x0)
LG200_CASE_END
