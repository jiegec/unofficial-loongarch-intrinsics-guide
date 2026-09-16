// atomic_sm_cmpswap_lt_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_cmpswap_lt_f32, 2)
#include "atomic_sm_cmpswap_lt_f32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_sm_cmpswap_lt_f32)
CASE(swap, 0x3f800000, 0x40000000, 0x3f800000, 0x40000000)
CASE(no-swap, 0x40400000, 0x40000000, 0x40400000, 0x40400000)
CASE(equal, 0x40000000, 0x40000000, 0x40000000, 0x40000000)
LG200_CASE_END
