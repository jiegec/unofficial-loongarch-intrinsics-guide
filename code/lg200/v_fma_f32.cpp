// v_fma_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fma_f32, 3)
#include "v_fma_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fma_f32)
CASE_FP(exact, rtne, 0x40000000, 0x40400000, 0x3f800000, 0x40e00000)
CASE_FP(inexact, rtne, 0x3fc00000, 0x40200000, 0x3fa00000, 0x40a00000)
LG200_CASE_END
