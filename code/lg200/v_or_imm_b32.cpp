// v_or_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_or_imm_b32, 2)
#include "v_or_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_or_imm_b32)
CASE(zero, 0x12345678, 0x0, 0x12345678)
CASE(mask, 0x12345678, 0xf0f0f0f0, 0xf2f4f6f8)
CASE(all, 0x12345678, 0xffffffff, 0xffffffff)
LG200_CASE_END
