// s_srl_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_srl_b32, 2)
#include "s_srl_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_srl_b32)
CASE(one, 0x2, 0x1, 0x1)
CASE(top-bit, 0x80000000, 0x1f, 0x1)
LG200_CASE_END
