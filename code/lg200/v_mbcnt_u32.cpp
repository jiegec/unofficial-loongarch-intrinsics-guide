// v_mbcnt_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mbcnt_u32, 2)
#include "v_mbcnt_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mbcnt_u32)
CASE(zero, 0x0, 0xff, 0xff)
CASE(basic, 0xf, 0xff, 0x1ff)
CASE(masked, 0xff, 0xf, 0x1f)
CASE(pc8-b8, 0xff, 0x8, 0x1f)
CASE(no-cover, 0xff, 0x100, 0x10f)
LG200_CASE_END
