// v_ldexp_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_ldexp_f32, 2)
#include "v_ldexp_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_ldexp_f32)
CASE_FP(scale-up, rtne, 0x3f800000, 0x2, 0x40800000)
CASE_FP(scale-down, rtne, 0x3f800000, 0xffffffff, 0x3f000000)
CASE_FP(inf-pos, rtne, 0x7f800000 0x3f800000, 0x7f800000)
CASE_FP(inf-neg, rtne, 0xff800000 0x3f800000, 0xff800000)
CASE_FP(zero-neg, rtne, 0x80000000 0x3f800000, 0x80000000)
CASE_FP(zero-pos, rtne, 0x0 0x3f800000, 0x00000000)
LG200_CASE_END
