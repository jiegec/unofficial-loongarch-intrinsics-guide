// s_wqm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_wqm_b32, 1)
#include "s_wqm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_wqm_b32)
CASE(zero, 0x0, 0x0)
CASE(bit, 0x2, 0xf)
CASE(two-quads, 0x55, 0xff)
CASE(mixed, 0x12345678, 0xffffffff)
LG200_CASE_END
