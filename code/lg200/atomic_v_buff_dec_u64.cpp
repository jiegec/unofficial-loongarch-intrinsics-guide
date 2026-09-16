// atomic_v_buff_dec_u64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_dec_u64, 4)
#include "atomic_v_buff_dec_u64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_v_buff_dec_u64)
CASE(basic, 0x5, 0x0, 0xa, 0x0, 0x5, 0x0, 0x4, 0x0)
CASE(zero, 0x0, 0x0, 0xffffffff, 0xffffffff, 0x0, 0x0, 0xffffffff, 0xffffffff)
CASE(at-max, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffe, 0xffffffff)
LG200_CASE_END
