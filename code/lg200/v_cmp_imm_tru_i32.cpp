// v_cmp_imm_tru_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_imm_tru_i32, 2)
#include "v_cmp_imm_tru_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_imm_tru_i32)
CASE(any, 0x0, 0x0, 0xffff)
CASE(any2, 0x13579bdf, 0x0, 0xffff)
LG200_CASE_END
