// v_subb_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_subb_u32, 3)
#include "v_subb_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_subb_u32)
CASE(basic, 0x5, 0x2, 0x0, 0x3)
CASE(borrow-ignored, 0x5, 0x2, 0x1, 0x3)
LG200_CASE_END
