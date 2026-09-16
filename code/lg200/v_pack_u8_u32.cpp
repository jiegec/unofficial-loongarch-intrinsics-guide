// v_pack_u8_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_u8_u32, 3)
#include "v_pack_u8_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_u8_u32)
CASE(zero, 0x0, 0x0, 0x0, 0x0)
CASE(basic, 0x12, 0x34, 0x0, 0x12)
CASE(truncate, 0x1122, 0x3344, 0x0, 0xff)
CASE(sel-byte2, 0x11, 0x2, 0x0, 0x110000)
CASE(keep-c, 0xaa, 0x3, 0xdeadbeef, 0xaaadbeef)
LG200_CASE_END
