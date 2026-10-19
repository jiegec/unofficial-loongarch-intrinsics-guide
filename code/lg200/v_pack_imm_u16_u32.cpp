// v_pack_imm_u16_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_imm_u16_u32, 2)
#include "v_pack_imm_u16_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_imm_u16_u32)
CASE(basic, 0x1234, 0x5678, 0x56781234)
CASE(big, 0x11223344, 0x1, 0x1ffff)
LG200_CASE_END
