// s_cmp_imm -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_cmp_imm, 2)
#include "s_cmp_imm.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_cmp_imm)
CASE(eq-true, 0x13579bdf, 0x13579bdf, 0xffff)
CASE(eq-false, 0x13579bde, 0x13579bdf, 0x0)
CASE(zero, 0x0, 0x0, 0xffff)
LG200_CASE_END
