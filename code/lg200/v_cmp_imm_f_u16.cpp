// v_cmp_imm_f_u16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_imm_f_u16, 2)
#include "v_cmp_imm_f_u16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_imm_f_u16)
CASE(any, 0x0, 0x0, 0x0)
CASE(any2, 0x1357, 0x0, 0x0)
LG200_CASE_END
