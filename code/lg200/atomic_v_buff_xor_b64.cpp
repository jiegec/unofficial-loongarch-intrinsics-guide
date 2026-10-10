// atomic_v_buff_xor_b64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_xor_b64, 4)
#include "atomic_v_buff_xor_b64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_v_buff_xor_b64)
CASE(identity, 0x9abcdef0, 0x12345678, 0x0, 0x0, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678)
CASE(invert, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x0, 0x0)
CASE(toggle, 0xf0f0f0f, 0xf0f0f0f, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f, 0xf0f0f0f, 0xffffffff, 0xffffffff)
LG200_CASE_END
