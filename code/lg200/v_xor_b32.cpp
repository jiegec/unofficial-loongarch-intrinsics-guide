// v_xor_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_xor_b32, 2)
#include "v_xor_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_xor_b32)
CASE(self-cancel, 0x12345678, 0x12345678, 0x0)
CASE(zero-identity, 0x0, 0x12345678, 0x12345678)
CASE(all-ones, 0xffffffff, 0x12345678, 0xedcba987)
CASE(mixed, 0x12345678, 0xf0f0f0f, 0x1d3b5977)
LG200_CASE_END
