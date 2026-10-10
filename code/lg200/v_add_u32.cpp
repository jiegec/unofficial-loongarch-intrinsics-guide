// v_add_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_add_u32, 2)
#include "v_add_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_add_u32)
CASE(zero-zero, 0x0, 0x0, 0x0, 0x0)
CASE(one-one, 0x1, 0x1, 0x2, 0x0)
CASE(max-one, 0xffffffff, 0x1, 0x0, 0x1)
CASE(max-max, 0xffffffff, 0xffffffff, 0xfffffffe, 0x1)
CASE(sign-wraparound, 0x80000000, 0x80000000, 0x0, 0x1)
LG200_CASE_END
