// v_bcnt0_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bcnt0_u32, 1)
#include "v_bcnt0_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bcnt0_u32)
CASE(zero, 0x0, 0x20)
CASE(all-ones, 0xffffffff, 0x0)
CASE(nibbles, 0xf0f0f0f, 0x10)
LG200_CASE_END
