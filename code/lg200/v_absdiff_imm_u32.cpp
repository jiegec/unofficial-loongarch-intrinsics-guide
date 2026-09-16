// v_absdiff_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_absdiff_imm_u32, 2)
#include "v_absdiff_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_absdiff_imm_u32)
CASE(equal, 0x5, 0x5, 0x0)
CASE(reverse, 0x4, 0x9, 0x5)
LG200_CASE_END
