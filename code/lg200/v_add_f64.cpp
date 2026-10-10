// v_add_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_add_f64, 4)
#include "v_add_f64.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(v_add_f64)
CASE_FP(one-plus-two, rtne, 0x0, 0x3ff00000, 0x0, 0x40000000, 0x0, 0x40080000)
CASE_FP(zero-plus, rtne, 0x0, 0x0, 0x0, 0x3ff00000, 0x0, 0x3ff00000)
// Subnormal operand inputs are flushed to zero (DAZ): 2^-1023 + 2^-1023 reads
// as 0 + 0, so the result is 0 (not the exact 2^-1022).
CASE_FP(subnormal, rtne, 0x0, 0x80000, 0x0, 0x80000, 0x0, 0x0)
CASE_FP(inf, rtne, 0x0, 0x7ff00000, 0x0, 0x3ff00000, 0x0, 0x7ff00000)
CASE_FP(neg-zero, rtne, 0x0, 0x80000000, 0x0, 0x0, 0x0, 0x0)
LG200_CASE_END
