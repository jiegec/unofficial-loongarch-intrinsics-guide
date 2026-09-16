// store_v_flat_b96 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_v_flat_b96, 3)
#include "store_v_flat_b96.h"
LG200_ORACLE_END(3)

LG200_CASE_BEGIN(store_v_flat_b96)
CASE(data-passthrough, 0x9abcdef0, 0x12345678, 0x55555555, 0x9abcdef0, 0x12345678, 0x55555555)
CASE(zero-data, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x0, 0x80000000, 0x80000000, 0x0, 0x80000000, 0x80000000)
LG200_CASE_END
