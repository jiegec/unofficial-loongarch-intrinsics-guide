// v_mul24_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul24_i32, 2)
#include "v_mul24_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul24_i32)
CASE(positive, 0x3, 0x4, 0xc)
CASE(negative, 0xffffff, 0x5, 0xfffffffb)
CASE(min24, 0x800000, 0x800000, 0x0)
LG200_CASE_END
