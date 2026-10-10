// load_v_buff_b96 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_v_buff_b96, 3)
#include "load_v_buff_b96.h"
LG200_ORACLE_END(3)

LG200_CASE_BEGIN(load_v_buff_b96)
CASE(data, 0x12345678, 0x9abcdef0, 0x55555555, 0x12345678, 0x9abcdef0, 0x55555555)
CASE(zero, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
CASE(mixed, 0xffffffff, 0x00000000, 0x80000000, 0xffffffff, 0x00000000, 0x80000000)
LG200_CASE_END
