// s_quadmask_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_quadmask_b32, 1)
#include "s_quadmask_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_quadmask_b32)
CASE(zero, 0x0, 0x0)
CASE(bit, 0x8, 0x1)
CASE(two-quads, 0x55, 0x3)
CASE(mixed, 0x12345678, 0xff)
LG200_CASE_END
