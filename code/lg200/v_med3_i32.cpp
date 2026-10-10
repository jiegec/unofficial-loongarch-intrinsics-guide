// v_med3_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_med3_i32, 3)
#include "v_med3_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_med3_i32)
CASE(middle, 0xffffffff, 0x1, 0x0, 0x0)
CASE(ordered, 0xfffffffd, 0xffffffff, 0x5, 0xffffffff)
LG200_CASE_END
