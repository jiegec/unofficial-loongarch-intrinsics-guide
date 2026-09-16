// atomic_v_buff_swap_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_swap_b32, 2)
#include "atomic_v_buff_swap_b32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_buff_swap_b32)
CASE(basic, 0x5, 0x9, 0x5, 0x9)
LG200_CASE_END
