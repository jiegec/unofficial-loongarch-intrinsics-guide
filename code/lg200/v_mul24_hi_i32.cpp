// v_mul24_hi_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul24_hi_i32, 2)
#include "v_mul24_hi_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul24_hi_i32)
CASE(small, 0x3, 0x4, 0x0)
CASE(negative, 0x800000, 0x800000, 0x4000)
CASE(positive-max, 0x7fffff, 0x7fffff, 0x3fff)
LG200_CASE_END
