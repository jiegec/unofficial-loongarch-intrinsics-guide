// v_srarev_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_srarev_imm_b32, 2)
#include "v_srarev_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_srarev_imm_b32)
CASE(basic, 0x4, 0xffffffff, 0xffffffff)
CASE(pos, 0x4, 0x100, 0x10)
LG200_CASE_END
