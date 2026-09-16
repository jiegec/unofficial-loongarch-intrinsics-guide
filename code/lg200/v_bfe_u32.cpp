// v_bfe_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bfe_u32, 3)
#include "v_bfe_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bfe_u32)
CASE(full-width, 0x12345678, 0x0, 0x10, 0x5678)
CASE(nibble, 0x12345678, 0x4, 0x8, 0x67)
CASE(width-zero, 0xffffffff, 0x0, 0x0, 0x0)
CASE(high, 0x80000000, 0x1f, 0x1, 0x1)
LG200_CASE_END
