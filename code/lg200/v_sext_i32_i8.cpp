// v_sext_i32_i8 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sext_i32_i8, 1)
#include "v_sext_i32_i8.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sext_i32_i8)
CASE(zero, 0x0, 0x0)
CASE(positive, 0x7f, 0x7f)
CASE(negative, 0x80, 0xffffff80)
CASE(truncate, 0x123456ff, 0xffffffff)
LG200_CASE_END
