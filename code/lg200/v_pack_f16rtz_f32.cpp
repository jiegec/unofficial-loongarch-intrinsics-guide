// v_pack_f16rtz_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_f16rtz_f32, 2)
#include "v_pack_f16rtz_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_f16rtz_f32)
CASE_FP(zero-zero, rtne|daz|ftz, 0x0, 0x0, 0x0)
CASE_FP(one-one, rtne|daz|ftz, 0x3f800000, 0x3f800000, 0x3c003c00)
CASE_FP(one-two, rtne|daz|ftz, 0x3f800000, 0x40000000, 0x40003c00)
CASE_FP(half-minus-one, rtne|daz|ftz, 0x3f000000, 0xbf800000, 0xbc003800)
CASE_FP(small-rtz, rtne|daz|ftz, 0x3c8efa35, 0x3c8efa35, 0x24772477)
CASE_FP(nan-pos, rtne, 0x7fc00000, 0x3f800000, 0x3c007e00)
CASE_FP(nan-neg, rtne, 0xffc00000, 0x3f800000, 0x3c00fe00)
CASE_FP(inf-pos, rtne, 0x7f800000, 0x3f800000, 0x3c007c00)
CASE_FP(inf-neg, rtne, 0xff800000, 0x3f800000, 0x3c00fc00)
CASE_FP(zero-neg, rtne, 0x80000000, 0x3f800000, 0x3c008000)
CASE_FP(subnormal-min, rtne, 0x1, 0x3f800000, 0x3c000000)
CASE_FP(subnormal-neg, rtne, 0x807fffff, 0x3f800000, 0x3c008000)
CASE_FP(nan-signal-pos, rtne, 0x7f800001, 0x3f800000, 0x3c007c00)
CASE_FP(nan-signal-neg, rtne, 0xff800001, 0x3f800000, 0x3c00fc00)
CASE_FP(finite-max-pos, rtne, 0x7f7fffff, 0x3f800000, 0x3c007bff)
CASE_FP(finite-max-neg, rtne, 0xff7fffff, 0x3f800000, 0x3c00fbff)
CASE_FP(finite-min-pos, rtne, 0x800000, 0x3f800000, 0x3c000000)
CASE_FP(finite-min-neg, rtne, 0x80800000, 0x3f800000, 0x3c008000)
LG200_CASE_END
