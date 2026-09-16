// v_pack_i16_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_i16_i32, 3)
#include "v_pack_i16_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_i16_i32)
CASE(zero, 0x0, 0x0, 0x0, 0x0)
CASE(negative, 0xffffffff, 0xffffffff, 0x0, 0xffffffff)
CASE(basic, 0x8000, 0x7fff, 0x0, 0x7fff7fff)
LG200_CASE_END
