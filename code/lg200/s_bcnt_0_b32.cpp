// s_bcnt_0_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_bcnt_0_b32, 1)
#include "s_bcnt_0_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_bcnt_0_b32)
CASE(zero, 0x0, 0x20)
CASE(all-ones, 0xffffffff, 0x0)
LG200_CASE_END
