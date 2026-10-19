// load_s_buff_w1 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_s_buff_w1, 2)
#include "load_s_buff_w1.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_s_buff_w1)
CASE(data-passthrough, 0x12345678, 0x0, 0x12345678)
CASE(zero-data, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
