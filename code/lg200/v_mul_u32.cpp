// v_mul_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul_u32, 2)
#include "v_mul_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul_u32)
CASE(zero, 0x0, 0x5, 0x0)
CASE(small, 0x3, 0x4, 0xc)
CASE(wrap, 0xffffffff, 0xffffffff, 0x1)
CASE(half, 0x80000000, 0x2, 0x0)
LG200_CASE_END
