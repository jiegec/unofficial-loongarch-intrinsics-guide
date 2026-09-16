// atomic_v_buff_and_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_and_b32, 2)
#include "atomic_v_buff_and_b32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_buff_and_b32)
CASE(mask, 0xf3, 0x5a, 0xf3, 0x52)
LG200_CASE_END
