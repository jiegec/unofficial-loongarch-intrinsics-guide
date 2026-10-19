// v_bfe_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bfe_i32, 3)
#include "v_bfe_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bfe_i32)
CASE(positive, 0x78, 0x0, 0x8, 0x78)
CASE(negative, 0xf0, 0x0, 0x8, 0xfffffff0)
CASE(width-zero, 0xffffffff, 0x0, 0x0, 0x0)
CASE(middle, 0x12345678, 0x4, 0xc, 0x567)
LG200_CASE_END
