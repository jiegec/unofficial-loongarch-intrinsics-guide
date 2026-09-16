// v_pack_i8_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_i8_i32, 3)
#include "v_pack_i8_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_i8_i32)
CASE(zero, 0x0, 0x0, 0x0, 0x0)
CASE(negative, 0xffffffff, 0xffffffff, 0x0, 0xff000000)
CASE(basic, 0xff, 0x80, 0x0, 0x7f)
CASE(sel-byte2, 0x11, 0x2, 0x0, 0x110000)
CASE(keep-c, 0x7f, 0x1, 0x12345678, 0x12347f78)
LG200_CASE_END
