// s_bitset0_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_bitset0_b32, 2)
#include "s_bitset0_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_bitset0_b32)
CASE(clear, 0x4, 0xffffffff, 0x4)
CASE(noop, 0x0, 0x0, 0x0)
LG200_CASE_END
