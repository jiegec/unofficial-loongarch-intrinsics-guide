// s_addc_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_addc_imm_u32, 3)
#include "s_addc_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_addc_imm_u32)
CASE(no-carry, 0x13579bdf, 0x1, 0x0, 0x13579be0)
CASE(carry-in, 0x13579bdf, 0x1, 0x1, 0x13579be1)
CASE(wrap, 0xffffffff, 0x1, 0x0, 0x0)
LG200_CASE_END
