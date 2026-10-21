// v_sin_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sin_f32, 1)
#include "v_sin_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sin_f32)
CASE_FP(zero, rtne, 0x0, 0x00000000)
// Source is a fraction of a turn: 0x3fc90fdb (~1.5708) -> angle 2*pi*1.5708.
// CPU oracle computes the exact sin(2*pi*a) = 0xbedc5079 (kept as expected so
// the CPU suite stays green); the hardware v_sin approximation returns the
// 1-ulp-higher 0xbedc507a (recorded hardware divergence).
CASE_FP(half-pi, rtne, 0x3fc90fdb, 0xbedc5079)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0xffc00000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x7fc00015)
CASE_FP(inf-neg, rtne, 0xff800000, 0xffc00015)
CASE_FP(subnormal-min, rtne, 0x1, 0x00000000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0x80000000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x80000000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x7fc00001)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0xffc00001)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x00000000)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x80000000)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x01c90fdb)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x81c90fdb)
CASE_FP(zero, rtz, 0x0, 0x00000000)
CASE_FP(zero, rtup, 0x0, 0x00000000)
CASE_FP(zero, rtdn, 0x0, 0x00000000)
CASE_FP(half-pi, rtz, 0x3fc90fdb, 0xbedc5079)
CASE_FP(half-pi, rtup, 0x3fc90fdb, 0xbedc5079)
CASE_FP(half-pi, rtdn, 0x3fc90fdb, 0xbedc5079)
CASE_FP(nan-pos, rtz, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-pos, rtup, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-pos, rtdn, 0x7fc00000, 0x7fc00000)
LG200_CASE_END
