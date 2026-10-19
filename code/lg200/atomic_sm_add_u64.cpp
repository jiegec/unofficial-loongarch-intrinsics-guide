// atomic_sm_add_u64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_add_u64, 4)
#include "atomic_sm_add_u64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_sm_add_u64)
CASE(basic, 0x9abcdef0, 0x12345678, 0x3, 0x0, 0x9abcdef0, 0x12345678, 0x9abcdef3, 0x12345678)
CASE(wrap, 0xffffffff, 0xffffffff, 0x1, 0x0, 0xffffffff, 0xffffffff, 0x0, 0x0)
CASE(carry-out, 0x0, 0x80000000, 0x0, 0x80000000, 0x0, 0x80000000, 0x0, 0x0)
LG200_CASE_END
