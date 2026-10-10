// v_absdiff_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_absdiff_i32, 2)
#include "v_absdiff_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_absdiff_i32)
CASE(equal, 0x5, 0x5, 0x0)
CASE(negative, 0xffffffff, 0x1, 0x2)
CASE(min-max, 0x80000000, 0x7fffffff, 0x1)
CASE(min-min, 0x80000000, 0x80000000, 0x0)
LG200_CASE_END
