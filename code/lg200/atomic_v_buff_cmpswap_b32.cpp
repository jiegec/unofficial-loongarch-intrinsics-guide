// atomic_v_buff_cmpswap_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_cmpswap_b32, 3)
#include "atomic_v_buff_cmpswap_b32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_buff_cmpswap_b32)
CASE(match, 0x7, 0x7, 0x63, 0x7, 0x63)
CASE(mismatch, 0x7, 0x8, 0x63, 0x7, 0x7)
CASE(extreme, 0x0, 0x0, 0xffffffff, 0x0, 0xffffffff)
LG200_CASE_END
