// v_mul_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul_f64, 4)
#include "v_mul_f64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_mul_f64)
CASE_FP(two-times-three, rtne, 0x0, 0x40000000, 0x0, 0x40080000, 0x0, 0x40180000)
CASE_FP(neg-times-pos, rtne, 0x0, 0xc0000000, 0x0, 0x40080000, 0x0, 0xc0180000)
CASE_FP(zero-times, rtne, 0x0, 0x0, 0x0, 0x40140000, 0x0, 0x0)
CASE_FP(overflow, rtne, 0x0, 0x7e800000, 0x0, 0x7e800000, 0x0, 0x7ff00000)
LG200_CASE_END
