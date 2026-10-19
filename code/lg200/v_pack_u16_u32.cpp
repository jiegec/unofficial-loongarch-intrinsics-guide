// v_pack_u16_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_u16_u32, 3)
#include "v_pack_u16_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_u16_u32)
CASE(zero, 0x0, 0x0, 0x0, 0x0)
CASE(basic, 0x1234, 0x5678, 0x0, 0x56781234)
CASE(truncate, 0x11223344, 0x55667788, 0x0, 0xffffffff)
LG200_CASE_END
