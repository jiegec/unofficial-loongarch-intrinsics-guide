// v_sext_i32_i16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sext_i32_i16, 1)
#include "v_sext_i32_i16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sext_i32_i16)
CASE(zero, 0x0, 0x0)
CASE(positive, 0x7fff, 0x7fff)
CASE(negative, 0x8000, 0xffff8000)
CASE(minus_one, 0xffff, 0xffffffff)
CASE(hi_garbage_neg, 0x12348000, 0xffff8000)
CASE(hi_garbage_pos, 0xdead7fff, 0x7fff)
CASE(hi_ones_low_zero, 0xffff0000, 0x0)
LG200_CASE_END
