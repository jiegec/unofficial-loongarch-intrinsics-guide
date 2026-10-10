// s_orn2_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_orn2_b32, 2)
#include "s_orn2_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_orn2_b32)
CASE(basic, 0xf0f0f0f, 0xf0f0f0f, 0xffffffff)
CASE(mixed, 0x12345678, 0xf0f0f0f, 0xf2f4f6f8)
LG200_CASE_END
