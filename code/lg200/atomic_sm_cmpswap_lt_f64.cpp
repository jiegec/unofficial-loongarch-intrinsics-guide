// atomic_sm_cmpswap_lt_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_cmpswap_lt_f64, 4)
#include "atomic_sm_cmpswap_lt_f64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_sm_cmpswap_lt_f64)
CASE(keep-lt, 0x0, 0x3ff00000, 0x0, 0x40000000, 0x0, 0x3ff00000, 0x0, 0x3ff00000)
CASE(no-swap, 0x0, 0x40080000, 0x0, 0x40000000, 0x0, 0x40080000, 0x0, 0x40080000)
CASE(nan, 0x0, 0x7ff80000, 0x0, 0x3ff00000, 0x0, 0x7ff80000, 0x0, 0x7ff80000)
LG200_CASE_END
