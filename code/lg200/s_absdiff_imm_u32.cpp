// s_absdiff_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_absdiff_imm_u32, 2)
#include "s_absdiff_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_absdiff_imm_u32)
CASE(equal, 0x13579bdf, 0x13579bdf, 0x0)
CASE(reverse, 0x13579bde, 0x13579bdf, 0x1)
CASE(extreme, 0x0, 0xffffffff, 0xffffffff)
LG200_CASE_END
