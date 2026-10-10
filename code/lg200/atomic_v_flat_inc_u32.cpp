// atomic_v_flat_inc_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_flat_inc_u32, 2)
#include "atomic_v_flat_inc_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_flat_inc_u32)
CASE(basic, 0x5, 0xa, 0x5, 0x6)
CASE(at-bound, 0xa, 0xa, 0xa, 0x0)
CASE(zero-bound, 0x0, 0x0, 0x0, 0x0)
CASE(from-zero, 0x0, 0x5, 0x0, 0x1)
LG200_CASE_END
