// v_subrev_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_subrev_u32, 2)
#include "v_subrev_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_subrev_u32)
CASE(basic, 0x1, 0x5, 0x4)
CASE(wrap, 0x5, 0x1, 0xfffffffc)
CASE(zero, 0x7, 0x7, 0x0)
CASE(reverse-mid-across-sign, 0x40000000, 0x80000000, 0x40000000)
CASE(reverse-adjacent-at-min, 0x80000000, 0x80000001, 0x1)
CASE(reverse-negate-adjacent-min, 0x0, 0x80000001, 0x80000001)
LG200_CASE_END
