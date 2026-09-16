// v_select_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_select_b32, 3)
#include "v_select_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_select_b32)
CASE(s1-c1, 0x1, 0x11111111, 0x1, 0x11111111)
CASE(s0-c1, 0x0, 0x33333333, 0x1, 0x33333333)
LG200_CASE_END
