// store_v_flat_b16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_v_flat_b16, 1)
#include "store_v_flat_b16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(store_v_flat_b16)
CASE(zero-low-half, 0x0, 0x0)
CASE(truncate-low-half, 0xbeef, 0xbeef)
LG200_CASE_END
