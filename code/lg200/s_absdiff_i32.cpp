// s_absdiff_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_absdiff_i32, 2)
#include "s_absdiff_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_absdiff_i32)
CASE(negative, 0xffffffff, 0x1, 0x2)
CASE(min-max, 0x80000000, 0x7fffffff, 0x1)
LG200_CASE_END
