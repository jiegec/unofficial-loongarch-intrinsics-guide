// atomic_v_buff_min_i64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_min_i64, 4)
#include "atomic_v_buff_min_i64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_v_buff_min_i64)
CASE(old-smaller, 0xfffffff8, 0xffffffff, 0x3, 0x0, 0xfffffff8, 0xffffffff, 0xfffffff8, 0xffffffff)
CASE(signed-bound, 0xffffffff, 0x7fffffff, 0x0, 0x80000000, 0xffffffff, 0x7fffffff, 0x0, 0x80000000)
LG200_CASE_END
