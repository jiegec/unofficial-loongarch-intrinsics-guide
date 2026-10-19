// s_sext_i32_i16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_sext_i32_i16, 1)
#include "s_sext_i32_i16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_sext_i32_i16)
CASE(negative, 0x8000, 0xffff8000)
CASE(positive, 0x7fff, 0x7fff)
LG200_CASE_END
