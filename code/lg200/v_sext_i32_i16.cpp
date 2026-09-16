// v_sext_i32_i16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sext_i32_i16, 1)
#include "v_sext_i32_i16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sext_i32_i16)
CASE(zero, 0x0, 0x0)
CASE(positive, 0x7fff, 0x7fff)
CASE(negative, 0x8000, 0xffff8000)
LG200_CASE_END
