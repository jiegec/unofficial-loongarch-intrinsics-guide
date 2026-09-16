// v_bitalign_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bitalign_b32, 3)
#include "v_bitalign_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bitalign_b32)
CASE(zero, 0x12345678, 0xaabbccdd, 0x0, 0x12345678)
CASE(one, 0x12345678, 0xaabbccdd, 0x1, 0x891a2b3c)
CASE(four, 0x12345678, 0xaabbccdd, 0x4, 0xd1234567)
LG200_CASE_END
