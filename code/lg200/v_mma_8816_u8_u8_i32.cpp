// v_mma_8816_u8_u8_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mma_8816_u8_u8_i32, 3)
#include "v_mma_8816_u8_u8_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mma_8816_u8_u8_i32)
CASE(ones-times-twos, 0x1010101, 0x2020202, 0x0, 0x20)
CASE(ones-ones, 0x1010101, 0x1010101, 0x0, 0x10)
CASE(single-byte, 0x1, 0x1, 0x0, 0x4)
CASE(accumulate, 0x1010101, 0x2020202, 0x7f7f7f7f, 0x7f7f7f9f)
CASE(neg-signed, 0xfffffffe, 0x2, 0x0, 0x7f0)
LG200_CASE_END
