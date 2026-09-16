// atomic_v_buff_dec_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_dec_u32, 2)
#include "atomic_v_buff_dec_u32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_buff_dec_u32)
CASE(basic, 0x5, 0xa, 0x5, 0x4)
CASE(zero, 0x0, 0xa, 0x0, 0xa)
CASE(above-bound, 0xb, 0xa, 0xb, 0xa)
CASE(bound-hit, 0xa, 0xa, 0xa, 0x9)
LG200_CASE_END
