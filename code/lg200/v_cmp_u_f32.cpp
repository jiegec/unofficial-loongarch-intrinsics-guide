// v_cmp_u_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_u_f32, 2)
#include "v_cmp_u_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_u_f32)
CASE(finite, 0x3f800000, 0x40000000, 0x0)
CASE(nan, 0x3f800000, 0x7fc00000, 0xffff)
CASE(both-nan, 0x7fc00000, 0x7fc00000, 0xffff)
LG200_CASE_END
