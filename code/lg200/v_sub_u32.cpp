// v_sub_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sub_u32, 2)
#include "v_sub_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_sub_u32)
CASE(zero-zero, 0x0, 0x0, 0x0, 0x0)
CASE(one-zero, 0x1, 0x0, 0x1, 0x0)
CASE(zero-one, 0x0, 0x1, 0xffffffff, 0x1)
CASE(max-max, 0xffffffff, 0xffffffff, 0x0, 0x0)
CASE(max-zero, 0xffffffff, 0x0, 0xffffffff, 0x0)
LG200_CASE_END
