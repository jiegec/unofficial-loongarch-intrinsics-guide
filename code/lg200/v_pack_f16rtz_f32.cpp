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
LG200_CASE_END
