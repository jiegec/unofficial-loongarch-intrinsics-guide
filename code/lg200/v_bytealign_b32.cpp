// v_bytealign_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bytealign_b32, 3)
#include "v_bytealign_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bytealign_b32)
CASE(zero, 0x12345678, 0xaabbccdd, 0x0, 0x12345678)
CASE(two, 0x12345678, 0xaabbccdd, 0x2, 0xccdd1234)
CASE(seven, 0x12345678, 0xaabbccdd, 0x7, 0xbbccdd12)
LG200_CASE_END
