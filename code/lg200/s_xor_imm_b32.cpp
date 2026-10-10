// s_xor_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_xor_imm_b32, 2)
#include "s_xor_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_xor_imm_b32)
CASE(zero, 0x13579bdf, 0x0, 0x13579bdf)
CASE(invert, 0x13579bdf, 0xffffffff, 0xeca86420)
CASE(self, 0x13579bdf, 0x13579bdf, 0x0)
LG200_CASE_END
