// store_v_buff_b128 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_v_buff_b128, 4)
#include "store_v_buff_b128.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(store_v_buff_b128)
CASE(data, 0x12345678, 0x9abcdef0, 0x55555555, 0xaaaaaaaa, 0x12345678, 0x9abcdef0, 0x55555555, 0xaaaaaaaa)
CASE(zero, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
CASE(all-ones, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff)
LG200_CASE_END
