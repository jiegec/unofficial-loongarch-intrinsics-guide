// v_ineg_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_ineg_i32, 1)
#include "v_ineg_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_ineg_i32)
CASE(zero, 0x0, 0x0)
CASE(one, 0x1, 0xffffffff)
CASE(minus-one, 0xffffffff, 0x1)
CASE(min-wraps, 0x80000000, 0x80000000)
LG200_CASE_END
