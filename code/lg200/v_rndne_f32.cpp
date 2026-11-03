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
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x7fc00000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0xffc00000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x7f800000)
CASE_FP(inf-neg, rtne, 0xff800000, 0xff800000)
CASE_FP(zero-neg, rtne, 0x80000000, 0x80000000)
CASE_FP(subnormal-min, rtne, 0x1, 0x00000000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0x80000000)
CASE_FP(zero-pos, rtne, 0x0, 0x00000000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x7fc00001)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0xffc00001)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x7f7fffff)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0xff7fffff)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x0)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x80000000)
CASE_FP(half-zero-pos, rtne, 0x3f000000, 0x00000000)
CASE_FP(half-zero-neg, rtne, 0xbf000000, 0x80000000)
CASE_FP(half-one-pos, rtne, 0x3fc00000, 0x40000000)
CASE_FP(half-one-neg, rtne, 0xbfc00000, 0xc0000000)
CASE_FP(integer, rtne, 0x40400000, 0x40400000)
CASE_FP(carry-top, rtne, 0x4affffff, 0x4b000000)
LG200_CASE_END
