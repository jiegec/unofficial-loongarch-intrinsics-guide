// store_v_buff_b64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_v_buff_b64, 2)
#include "store_v_buff_b64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(store_v_buff_b64)
CASE(data, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0)
CASE(zero, 0x0, 0x0, 0x0, 0x0)
CASE(sign-hi, 0x00000000, 0x80000000, 0x00000000, 0x80000000)
LG200_CASE_END
