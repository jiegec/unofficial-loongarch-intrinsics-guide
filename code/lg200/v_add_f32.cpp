// v_add_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_add_f32, 2)
#include "v_add_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_add_f32)
CASE_FP(zero-plus-zero, rtne, 0x0, 0x0, 0x0)
CASE_FP(one-plus-two, rtne, 0x3f800000, 0x40000000, 0x40400000)
// Hardware runs a fixed FP default (RN + denormal-flush + positive NaN) that
// the PS carrier cannot change, so rounding/flush policy tokens are ignored.
CASE_FP(subnormal, rtne, 0x1, 0x1, 0x0)
CASE_FP(subnormal-flush, rtne|ftz, 0x1, 0x1, 0x0)
CASE_FP(inf-plus, rtne, 0x7f800000, 0x3f800000, 0x7f800000)
CASE_FP(inf-minus-inf, rtne, 0x7f800000, 0xff800000, 0x7fc00000)
CASE_FP(daz, rtne|daz, 0x1, 0x3f800000, 0x3f800000)
CASE_FP(rtz-rounding, rtne, 0x3dcccccd, 0x3e4ccccd, 0x3e99999a)
CASE_FP(rtdn-rounding, rtne, 0x3f800000, 0x33000000, 0x3f800000)
CASE_FP(rtup-rounding, rtne, 0x3f800000, 0x33000000, 0x3f800000)
CASE_FP(neg-zero, rtne, 0x80000000, 0x0, 0x0)
// Boundary classes added 2026-10-13.  Their expected values were computed BY THIS
// ROW'S MODEL (fp_bounds_gen.py drives the compiled runner with a deliberately wrong
// expectation and reads back what the model answers), so they PIN the documented
// behaviour rather than independently validate it - the independent check is the
// hardware run.  Note nan-neg: the model canonicalises a negative quiet NaN to
// 0x7fc00000, while the live hardware has been seen returning 0xffc00000 for a
// negative quiet NaN input, so this case is expected to FAIL on hardware until that
// discrepancy is settled.  It is recorded deliberately, not overlooked.
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x3f800000, 0x7fc00000)
CASE_FP(nan-neg, rtne, 0xffc00000, 0x3f800000, 0x7fc00000)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x3f800000, 0x7f800000)
CASE_FP(inf-neg, rtne, 0xff800000, 0x3f800000, 0xff800000)
CASE_FP(subnormal-min, rtne, 0x1, 0x3f800000, 0x3f800000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0x3f800000, 0x3f800000)
CASE_FP(neg-zero-plus-one, rtne, 0x80000000, 0x3f800000, 0x3f800000)
LG200_CASE_END
