// v_cmp_eq_u16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_eq_u16, 2)
#include "v_cmp_eq_u16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_eq_u16)
CASE(zero, 0x0, 0x0, 0xffff)
CASE(high, 0xffff, 0xffff, 0xffff)
CASE(diff, 0x1, 0x2, 0x0)
LG200_CASE_END
