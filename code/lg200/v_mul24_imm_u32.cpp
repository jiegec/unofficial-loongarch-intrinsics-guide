// v_mul24_imm_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul24_imm_u32, 2)
#include "v_mul24_imm_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul24_imm_u32)
CASE(zero, 0x0, 0x5, 0x0)
CASE(small, 0x100, 0x100, 0x10000)
CASE(max, 0xffffff, 0xffffff, 0xfe000001)
LG200_CASE_END
