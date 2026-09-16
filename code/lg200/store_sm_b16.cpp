// store_sm_b16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_sm_b16, 1)
#include "store_sm_b16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(store_sm_b16)
CASE(high-half, 0xbeef, 0xbeef)
CASE(zero-data, 0x0, 0x0)
LG200_CASE_END
