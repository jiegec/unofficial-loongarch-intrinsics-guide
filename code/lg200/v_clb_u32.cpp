// v_clb_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_clb_u32, 1)
#include "v_clb_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_clb_u32)
CASE(positive, 0xffff, 0x10)
CASE(negative, 0xffff0000, 0x10)
CASE(zero, 0x0, 0x20)
LG200_CASE_END
