// atomic_v_buff_max_u64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_max_u64, 4)
#include "atomic_v_buff_max_u64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_v_buff_max_u64)
CASE(operand-larger, 0x5, 0x0, 0x9, 0x0, 0x5, 0x0, 0x9, 0x0)
CASE(max, 0x0, 0x80000000, 0xffffffff, 0xffffffff, 0x0, 0x80000000, 0xffffffff, 0xffffffff)
LG200_CASE_END
