// v_rndne_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_rndne_f32, 1)
#include "v_rndne_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_rndne_f32)
CASE_FP(half-up, rtne, 0x40200000, 0x40000000)
CASE_FP(half-down, rtne, 0x40600000, 0x40800000)
CASE_FP(negative, rtne, 0xc0200000, 0xc0000000)
CASE_FP(frac-neg, rtne, 0xbe800000, 0x80000000)
LG200_CASE_END
