// v_cmp_imm_eq_i16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_imm_eq_i16, 2)
#include "v_cmp_imm_eq_i16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_imm_eq_i16)
CASE(match, 0x1357, 0x1357, 0xffff)
CASE(zero, 0x0, 0x0, 0xffff)
CASE(mismatch, 0x1, 0x0, 0x0)
LG200_CASE_END
