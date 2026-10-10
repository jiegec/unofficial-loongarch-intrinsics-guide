// v_cmp_lt_i16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_lt_i16, 2)
#include "v_cmp_lt_i16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_lt_i16)
CASE(neg, 0xffff, 0x0, 0xffff)
CASE(pos, 0x1, 0x0, 0x0)
CASE(min, 0x8000, 0x1, 0xffff)
LG200_CASE_END
