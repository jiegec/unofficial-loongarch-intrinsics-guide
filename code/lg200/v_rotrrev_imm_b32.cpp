// v_rotrrev_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_rotrrev_imm_b32, 2)
#include "v_rotrrev_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_rotrrev_imm_b32)
CASE(basic, 0x1, 0x80000000, 0x40000000)
CASE(wrap, 0x1, 0x1, 0x80000000)
LG200_CASE_END
