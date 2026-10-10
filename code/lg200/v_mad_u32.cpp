// v_mad_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mad_u32, 3)
#include "v_mad_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mad_u32)
CASE(zero, 0x0, 0x5, 0x3, 0x3)
CASE(basic, 0x2, 0x3, 0x1, 0x7)
CASE(wrap, 0xffffffff, 0xffffffff, 0x1, 0x2)
LG200_CASE_END
