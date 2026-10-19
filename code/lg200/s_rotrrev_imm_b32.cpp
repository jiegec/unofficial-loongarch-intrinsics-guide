// s_rotrrev_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_rotrrev_imm_b32, 2)
#include "s_rotrrev_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_rotrrev_imm_b32)
CASE(one, 0x1, 0x1, 0x80000000)
CASE(four, 0x4, 0x12345678, 0x81234567)
CASE(masked-count, 0x21, 0x1, 0x80000000)
LG200_CASE_END
