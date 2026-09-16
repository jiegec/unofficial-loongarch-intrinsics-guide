// v_sqrt_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sqrt_f64, 2)
#include "v_sqrt_f64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_sqrt_f64)
CASE_FP(four, rtne, 0x0, 0x40100000, 0x0, 0x40000000)
CASE_FP(two, rtne, 0x0, 0x40000000, 0x667f3bcd, 0x3ff6a09e)
CASE_FP(three, rtne, 0x0, 0x40080000, 0xe8584caa, 0x3ffbb67a)
CASE_FP(perfect-square, rtne, 0x0, 0x40020000, 0x0, 0x3ff80000)
CASE_FP(subnormal, rtne, 0x1, 0x0, 0x0, 0x1e600000)
CASE_FP(tiny, rtne, 0x88d, 0x0, 0x3b88e5b3, 0x1eb764b4)
LG200_CASE_END
