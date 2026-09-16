// s_cmp_gt_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_cmp_gt_i32, 2)
#include "s_cmp_gt_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_cmp_gt_i32)
CASE(negative, 0x0, 0xffffffff, 0xffff)
CASE(positive, 0x13579bdf, 0x13579bde, 0xffff)
CASE(sign-mix, 0x80000000, 0x7fffffff, 0x0)
LG200_CASE_END
