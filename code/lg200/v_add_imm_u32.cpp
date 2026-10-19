// v_add_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_add_imm_u32, 2)
#include "v_add_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_add_imm_u32)
CASE(zero-plus-five, 0x0, 0x5, 0x5)
CASE(ten-plus-five, 0xa, 0x5, 0xf)
CASE(max-plus-five, 0xffffffff, 0x5, 0x4)
CASE(sign-plus-five, 0x80000000, 0x5, 0x80000005)
LG200_CASE_END
