// v_pack_imm_i16_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_imm_i16_i32, 2)
#include "v_pack_imm_i16_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_imm_i16_i32)
CASE(basic, 0x8000, 0x7fff, 0x7fff7fff)
CASE(zero, 0x0, 0x0, 0x0)
LG200_CASE_END
