// v_bcnt1_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bcnt1_u32, 1)
#include "v_bcnt1_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bcnt1_u32)
CASE(zero, 0x0, 0x0)
CASE(one, 0x1, 0x1)
CASE(high-bit, 0x80000000, 0x1)
CASE(all-ones, 0xffffffff, 0x20)
CASE(nibbles, 0xf0f0f0f, 0x10)
CASE(mixed, 0x12345678, 0xd)
LG200_CASE_END
