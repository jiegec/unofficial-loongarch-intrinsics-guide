// v_sllrev_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sllrev_imm_b32, 2)
#include "v_sllrev_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sllrev_imm_b32)
CASE(basic, 0x4, 0x1, 0x10)
CASE(zero, 0x0, 0x5, 0x5)
LG200_CASE_END
