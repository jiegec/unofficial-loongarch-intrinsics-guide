// v_clz_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_clz_u32, 1)
#include "v_clz_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_clz_u32)
CASE(zero, 0x0, 0x20)
CASE(one, 0x1, 0x1f)
CASE(high-bit, 0x80000000, 0x0)
CASE(below-high, 0x7fffffff, 0x1)
CASE(all-ones, 0xffffffff, 0x0)
CASE(eight-ones, 0xffffff, 0x8)
LG200_CASE_END
