// s_bitcmp1_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_bitcmp1_b32, 2)
#include "s_bitcmp1_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_bitcmp1_b32)
CASE(set, 0x10, 0x4, 0xffff)
CASE(clear, 0x10, 0x5, 0x0)
LG200_CASE_END
