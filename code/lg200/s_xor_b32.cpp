// s_xor_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_xor_b32, 2)
#include "s_xor_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_xor_b32)
CASE(cancel, 0x12345678, 0x12345678, 0x0)
CASE(invert, 0x12345678, 0xffffffff, 0xedcba987)
LG200_CASE_END
