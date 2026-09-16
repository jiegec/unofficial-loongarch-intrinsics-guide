// atomic_sm_max_i64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_sm_max_i64, 4)
#include "atomic_sm_max_i64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_sm_max_i64)
CASE(old-larger, 0xfffffffc, 0xffffffff, 0xfffffffb, 0xffffffff, 0xfffffffc, 0xffffffff, 0xfffffffc, 0xffffffff)
CASE(operand-larger, 0xfffffffc, 0xffffffff, 0x3, 0x0, 0xfffffffc, 0xffffffff, 0x3, 0x0)
LG200_CASE_END
