// s_mul_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_mul_imm_i32, 2)
#include "s_mul_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_mul_imm_i32)
CASE(anchor, 0x13579bdf, 0x2, 0x26af37be)
CASE(negative, 0xfffffffe, 0x3, 0xfffffffa)
CASE(wrap, 0xffffffff, 0xffffffff, 0x1)
LG200_CASE_END
