// s_nor_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_nor_b32, 2)
#include "s_nor_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_nor_b32)
CASE(masks, 0xffff, 0xffff0000, 0x0)
CASE(basic, 0x12345678, 0xf0f0f0f, 0xe0c0a080)
LG200_CASE_END
