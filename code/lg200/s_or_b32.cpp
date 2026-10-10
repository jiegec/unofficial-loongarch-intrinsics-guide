// s_or_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_or_b32, 2)
#include "s_or_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_or_b32)
CASE(masks, 0xffff, 0xffff0000, 0xffffffff)
CASE(identity, 0x0, 0x12345678, 0x12345678)
LG200_CASE_END
