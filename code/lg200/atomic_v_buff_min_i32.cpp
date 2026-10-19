// atomic_v_buff_min_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_min_i32, 2)
#include "atomic_v_buff_min_i32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_buff_min_i32)
CASE(signedness, 0xfffffffe, 0x5, 0xfffffffe, 0xfffffffe)
LG200_CASE_END
