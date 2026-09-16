// v_interp_1_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_interp_1_f32, 2)
#include "v_interp_1_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_interp_1_f32)
CASE(one-two, 0x3f800000, 0x40000000, 0x40800000)
CASE(half-two, 0x3f000000, 0x40000000, 0x40400000)
CASE(two-two, 0x40000000, 0x40000000, 0x40c00000)
CASE(three-three, 0x40400000, 0x40400000, 0x41400000)
CASE(zero-two, 0x0, 0x40000000, 0x40000000)
CASE(two-zero, 0x40000000, 0x0, 0x0)
LG200_CASE_END
