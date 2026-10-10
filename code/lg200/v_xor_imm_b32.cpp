// v_xor_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_xor_imm_b32, 2)
#include "v_xor_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_xor_imm_b32)
CASE(zero, 0x12345678, 0x0, 0x12345678)
CASE(invert, 0x12345678, 0xffffffff, 0xedcba987)
CASE(nibbles, 0x12345678, 0xf0f0f0f0, 0xe2c4a688)
LG200_CASE_END
