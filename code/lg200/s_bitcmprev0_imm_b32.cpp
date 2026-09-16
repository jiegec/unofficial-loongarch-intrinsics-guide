// s_bitcmprev0_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_bitcmprev0_imm_b32, 2)
#include "s_bitcmprev0_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_bitcmprev0_imm_b32)
CASE(bit-set, 0x4, 0x10, 0x0)
CASE(bit-clear, 0x4, 0xef, 0xffff)
LG200_CASE_END
