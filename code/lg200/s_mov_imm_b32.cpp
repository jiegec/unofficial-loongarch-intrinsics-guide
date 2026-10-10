// s_mov_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_mov_imm_b32, 1)
#include "s_mov_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_mov_imm_b32)
CASE(zero, 0x0, 0x0)
CASE(max, 0xffffffff, 0xffffffff)
CASE(float-bits, 0x3f800000, 0x3f800000)
LG200_CASE_END
