// v_dp4a_u8_i8_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_dp4a_u8_i8_i32, 2)
#include "v_dp4a_u8_i8_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_dp4a_u8_i8_i32)
CASE(zero, 0x0, 0x0, 0x0)
CASE(basic, 0xff, 0xff, 0xffffff01)
CASE(order, 0xff, 0x80, 0xffff8080)
LG200_CASE_END
