// v_absdiff_imm_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_absdiff_imm_i32, 2)
#include "v_absdiff_imm_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_absdiff_imm_i32)
CASE(equal, 0x5, 0x5, 0x0)
CASE(negative, 0xffffffff, 0x1, 0x2)
LG200_CASE_END
