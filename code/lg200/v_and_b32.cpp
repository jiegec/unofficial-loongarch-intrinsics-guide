// v_and_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_and_b32, 2)
#include "v_and_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_and_b32)
CASE(zero-mask, 0x0, 0xffffffff, 0x0)
CASE(mask-zero, 0xffffffff, 0x0, 0x0)
CASE(identity, 0xffffffff, 0xffffffff, 0xffffffff)
CASE(nibbles, 0xf0f0f0f0, 0xf0f0f0f, 0x0)
CASE(mixed, 0x12345678, 0xf0f0f0f, 0x2040608)
LG200_CASE_END
