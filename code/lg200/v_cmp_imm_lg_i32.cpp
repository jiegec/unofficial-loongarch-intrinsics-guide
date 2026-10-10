// v_cmp_imm_lg_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_imm_lg_i32, 2)
#include "v_cmp_imm_lg_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_imm_lg_i32)
CASE(diff, 0x1, 0x0, 0xffff)
CASE(match, 0x1, 0x1, 0x0)
LG200_CASE_END
