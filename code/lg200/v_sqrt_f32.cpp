// v_sqrt_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sqrt_f32, 1)
#include "v_sqrt_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sqrt_f32)
CASE_FP(perfect, rtne, 0x40800000, 0x40000000)
// Hardware v_sqrt is approximate: for sqrt(2) it returns 0x3fb504f4 (1 ulp
// above the CPU's correctly-rounded 0x3fb504f3) and for sqrt(-x) it returns
// the qNaN payload 0xffc00014 (vs canonical 0xffc00000). Expected values below
// are the CPU oracle's (IEEE) results, so the CPU suite stays green; the GPU
// divergence is the recorded hardware approximation. rtup rounds to the same
// 0x3fb504f4 as the hardware.
CASE_FP(two, rtne, 0x40000000, 0x3fb504f3)
CASE_FP(zero, rtne, 0x0, 0x0)
CASE_FP(negative-zero, rtne, 0x80000000, 0x80000000)
CASE_FP(negative, rtne, 0xc0800000, 0xffc00014)  // [HW] fixed neg-NaN
CASE_FP(subnormal, rtne, 0x800000, 0x20000000)
CASE_FP(rtz, rtz, 0x40000000, 0x3fb504f3)
CASE_FP(rtup, rtup, 0x40000000, 0x3fb504f4)
LG200_CASE_END
