// store_v_flat_b64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_v_flat_b64, 2)
#include "store_v_flat_b64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(store_v_flat_b64)
CASE(data-passthrough, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678)
CASE(zero-data, 0x0, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x0, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
