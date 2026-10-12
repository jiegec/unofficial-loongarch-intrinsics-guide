// v_cubeface_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cubeface_f32, 3)
#include "v_cubeface_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cubeface_f32)
CASE_FP(data, rtne|daz|ftz, 0x3f800000, 0x0, 0x0, 0x0)
CASE_FP(py, rtne|daz|ftz, 0x0, 0x3f800000, 0x0, 0x40000000)
CASE_FP(pz, rtne|daz|ftz, 0x0, 0x0, 0x3f800000, 0x40800000)
CASE_FP(nx, rtne|daz|ftz, 0xbf800000, 0x0, 0x0, 0x3f800000)
CASE_FP(ny, rtne|daz|ftz, 0x0, 0xbf800000, 0x0, 0x40400000)
CASE_FP(nz, rtne|daz|ftz, 0x0, 0x0, 0xbf800000, 0x40a00000)
LG200_CASE_END
