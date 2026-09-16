// v_mul24_hi_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul24_hi_u32, 2)
#include "v_mul24_hi_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul24_hi_u32)
CASE(small, 0x3, 0x4, 0x0)
CASE(max24, 0xffffff, 0xffffff, 0xffff)
CASE(high, 0x800000, 0x10000, 0x80)
LG200_CASE_END
