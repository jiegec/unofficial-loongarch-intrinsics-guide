// s_and_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_and_b32, 2)
#include "s_and_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_and_b32)
CASE(mask, 0xf0f0f0f0, 0xf0f0f0f, 0x0)
CASE(identity, 0xffffffff, 0x12345678, 0x12345678)
LG200_CASE_END
