// s_bcnt_1_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_bcnt_1_b32, 1)
#include "s_bcnt_1_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_bcnt_1_b32)
CASE(zero, 0x0, 0x0)
CASE(mask-7, 0x7, 0x3)
CASE(mask-ffff, 0xffff, 0x10)
CASE(all-ones, 0xffffffff, 0x20)
LG200_CASE_END
