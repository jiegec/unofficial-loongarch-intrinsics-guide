// v_mad24_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mad24_i32, 3)
#include "v_mad24_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mad24_i32)
CASE(zero, 0x0, 0x5, 0x3, 0x3)
CASE(basic, 0x100, 0x100, 0x1, 0x10001)
LG200_CASE_END
