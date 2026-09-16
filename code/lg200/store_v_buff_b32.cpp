// store_v_buff_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_v_buff_b32, 1)
#include "store_v_buff_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(store_v_buff_b32)
CASE(data-passthrough, 0x3, 0x3)
CASE(zero-data, 0x0, 0x0)
CASE(sign-bit-data, 0x80000000, 0x80000000)
LG200_CASE_END
