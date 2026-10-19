// v_cmp_imm_lt_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_imm_lt_f16, 2)
#include "v_cmp_imm_lt_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_imm_lt_f16)
CASE(less, 0x3c00, 0x4000, 0xffff)
CASE(nan, 0x7e00, 0x3c00, 0x0)
CASE(equal, 0x3c00, 0x3c00, 0x0)
LG200_CASE_END
