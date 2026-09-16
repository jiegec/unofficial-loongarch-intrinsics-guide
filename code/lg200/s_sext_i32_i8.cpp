// s_sext_i32_i8 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_sext_i32_i8, 1)
#include "s_sext_i32_i8.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_sext_i32_i8)
CASE(negative, 0x80, 0xffffff80)
CASE(positive, 0x7f, 0x7f)
LG200_CASE_END
