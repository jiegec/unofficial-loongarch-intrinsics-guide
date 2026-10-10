// v_cmp_ngt_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmp_ngt_f64, 4)
#include "v_cmp_ngt_f64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmp_ngt_f64)
CASE(nan, 0x0, 0x7ff80000, 0x0, 0x3ff00000, 0xffff)
CASE(le, 0x0, 0x3ff00000, 0x0, 0x40000000, 0x0)
CASE(gt, 0x0, 0x40000000, 0x0, 0x3ff00000, 0x0)
LG200_CASE_END
