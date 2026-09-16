// atomic_v_flat_and_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_flat_and_b32, 2)
#include "atomic_v_flat_and_b32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_flat_and_b32)
CASE(keep, 0x12345678, 0xffffffff, 0x12345678, 0x12345678)
CASE(clear, 0xffffffff, 0x0, 0xffffffff, 0x0)
CASE(mask, 0xf0f0f0f, 0xff00ff00, 0xf0f0f0f, 0xf000f00)
LG200_CASE_END
