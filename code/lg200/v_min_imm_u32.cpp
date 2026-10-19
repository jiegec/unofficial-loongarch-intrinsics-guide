// v_min_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_min_imm_u32, 2)
#include "v_min_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_min_imm_u32)
CASE(below, 0x5, 0xa, 0x5)
CASE(above, 0xf, 0xa, 0xa)
LG200_CASE_END
