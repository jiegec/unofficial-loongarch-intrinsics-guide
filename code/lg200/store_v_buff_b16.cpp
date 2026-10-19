// store_v_buff_b16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_v_buff_b16, 1)
#include "store_v_buff_b16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(store_v_buff_b16)
CASE(low-half, 0xbeef, 0xbeef)
CASE(high-half, 0xcafe, 0xcafe)
CASE(vaddr-plus-imm, 0x1234, 0x1234)
LG200_CASE_END
