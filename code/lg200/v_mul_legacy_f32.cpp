// v_mul_legacy_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mul_legacy_f32, 2)
#include "v_mul_legacy_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mul_legacy_f32)
CASE_FP(two-times-three, rtne, 0x40000000, 0x40400000, 0x40c00000)
CASE_FP(neg, rtne, 0x40000000, 0xbf800000, 0xc0000000)
LG200_CASE_END
