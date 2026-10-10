// atomic_v_flat_and_b64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_flat_and_b64, 4)
#include "atomic_v_flat_and_b64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_v_flat_and_b64)
CASE(keep, 0x23456789, 0xabcdef01, 0xffffffff, 0xffffffff, 0x23456789, 0xabcdef01, 0x23456789, 0xabcdef01)
CASE(clear-low, 0xffffffff, 0xffffffff, 0x0, 0xffffffff, 0xffffffff, 0xffffffff, 0x0, 0xffffffff)
CASE(mask, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f, 0xf0f0f0f, 0xf0f0f0f0, 0xf0f0f0f0, 0x0, 0x0)
LG200_CASE_END
