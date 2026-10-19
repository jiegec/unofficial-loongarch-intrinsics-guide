// v_movindex_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_movindex_b32, 3)
#include "v_movindex_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_movindex_b32)
CASE(index-0, 0x11111111, 0x22222222, 0x0, 0x0)
CASE(index-1, 0x11111111, 0x22222222, 0x1, 0x0)
CASE(index-ff, 0x11111111, 0x22222222, 0xff, 0x0)
LG200_CASE_END
