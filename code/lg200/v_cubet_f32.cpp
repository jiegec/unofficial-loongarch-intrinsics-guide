// v_cubet_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cubet_f32, 3)
#include "v_cubet_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cubet_f32)
CASE_FP(data, rtne|daz|ftz, 0x3f800000, 0x0, 0x0, 0x80000000)
CASE_FP(s1t0r0, rtne|daz|ftz, 0x3f800000, 0x0, 0x0, 0x80000000)
CASE_FP(s0t1r0, rtne|daz|ftz, 0x0, 0x3f800000, 0x0, 0x0)
CASE_FP(s0t0r1, rtne|daz|ftz, 0x0, 0x0, 0x3f800000, 0x80000000)
CASE_FP(negy, rtne|daz|ftz, 0x0, 0xbf800000, 0x0, 0x80000000)
CASE_FP(negz, rtne|daz|ftz, 0x0, 0x0, 0xbf800000, 0x80000000)
CASE_FP(negx, rtne|daz|ftz, 0xbf800000, 0x0, 0x0, 0x80000000)
CASE_FP(ty, rtne|daz|ftz, 0x0, 0x3f800000, 0x3f000000, 0x3f000000)
CASE_FP(posmix, rtne|daz|ftz, 0x3f800000, 0x3f000000, 0x3e800000, 0xbf000000)
CASE_FP(mix2, rtne|daz|ftz, 0x3f800000, 0xbf000000, 0x3e800000, 0x3f000000)
CASE_FP(x-t075-r025, rtne|daz|ftz, 0x3f800000, 0x3f400000, 0x3e800000, 0xbf400000)
CASE_FP(x-t05-r075, rtne|daz|ftz, 0x3f800000, 0x3f000000, 0x3f400000, 0xbf000000)
CASE_FP(x-t-075-r025, rtne|daz|ftz, 0x3f800000, 0xbf400000, 0x3e800000, 0x3f400000)
CASE_FP(nx-t05-r025, rtne|daz|ftz, 0xbf800000, 0x3f000000, 0x3e800000, 0xbf000000)
CASE_FP(nx-t-05-r075, rtne|daz|ftz, 0xbf800000, 0xbf000000, 0x3f400000, 0x3f000000)
CASE_FP(y-s05-r025, rtne|daz|ftz, 0x3f000000, 0x3f800000, 0x3e800000, 0x3e800000)
CASE_FP(y-s-05-r075, rtne|daz|ftz, 0xbf000000, 0x3f800000, 0x3f400000, 0x3f400000)
CASE_FP(y-s075-r-025, rtne|daz|ftz, 0x3f400000, 0x3f800000, 0xbe800000, 0xbe800000)
CASE_FP(ny-s05-r025, rtne|daz|ftz, 0x3f000000, 0xbf800000, 0x3e800000, 0xbe800000)
CASE_FP(ny-s-025-r05, rtne|daz|ftz, 0xbe800000, 0xbf800000, 0x3f000000, 0xbf000000)
CASE_FP(z-s05-t025, rtne|daz|ftz, 0x3f000000, 0x3e800000, 0x3f800000, 0xbe800000)
CASE_FP(z-s-05-t075, rtne|daz|ftz, 0xbf000000, 0x3f400000, 0x3f800000, 0xbf400000)
CASE_FP(nz-s05-t025, rtne|daz|ftz, 0x3f000000, 0x3e800000, 0xbf800000, 0xbe800000)
CASE_FP(nz-s025-t-05, rtne|daz|ftz, 0x3e800000, 0xbf000000, 0xbf800000, 0x3f000000)
LG200_CASE_END
