// store_sm_b8 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_sm_b8, 1)
#include "store_sm_b8.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(store_sm_b8)
CASE(middle-byte, 0xaa, 0xaa)
CASE(zero-data, 0x0, 0x0)
LG200_CASE_END
