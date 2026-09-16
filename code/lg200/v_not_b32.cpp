// v_not_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_not_b32, 1)
#include "v_not_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_not_b32)
CASE(zero, 0x0, 0xffffffff)
CASE(all, 0xffffffff, 0x0)
CASE(mixed, 0xf0f0f0f, 0xf0f0f0f0)
LG200_CASE_END
