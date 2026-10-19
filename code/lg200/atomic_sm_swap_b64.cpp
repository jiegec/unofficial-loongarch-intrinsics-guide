// atomic_sm_swap_b64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_swap_b64, 4)
#include "atomic_sm_swap_b64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_sm_swap_b64)
CASE(basic, 0x9abcdef0, 0x12345678, 0x76543210, 0xfedcba98, 0x9abcdef0, 0x12345678, 0x76543210, 0xfedcba98)
CASE(zero, 0x0, 0x0, 0xffffffff, 0xffffffff, 0x0, 0x0, 0xffffffff, 0xffffffff)
LG200_CASE_END
