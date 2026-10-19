// v_or_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_or_b32, 2)
#include "v_or_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_or_b32)
CASE(zero-identity, 0x0, 0x12345678, 0x12345678)
CASE(all-ones, 0xffffffff, 0x12345678, 0xffffffff)
CASE(half-masks, 0xffff, 0xffff0000, 0xffffffff)
CASE(mixed, 0x12345678, 0xf0f0f0f, 0x1f3f5f7f)
LG200_CASE_END
