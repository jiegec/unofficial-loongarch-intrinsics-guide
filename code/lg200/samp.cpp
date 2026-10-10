// samp -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(samp, 2)
#include "samp.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(samp)
CASE(data-passthrough, 0x12345678, 0x0, 0x12345678)
CASE(zero-data, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x80000000, 0x0, 0x80000000)
// NEAREST/CLAMP-TO-EDGE pixel-center oracle: row i samples u=(2i+1)/16 and
// must land on texel i (16-lane uniform fetch => deterministic compare).
CASE(u1, 0x10000001, 0x0, 0x10000001)
CASE(u2, 0x10000002, 0x0, 0x10000002)
CASE(u3, 0x10000003, 0x0, 0x10000003)
CASE(u4, 0x10000004, 0x0, 0x10000004)
CASE(u5, 0x10000005, 0x0, 0x10000005)
CASE(u6, 0x10000006, 0x0, 0x10000006)
CASE(u7, 0x10000007, 0x0, 0x10000007)
LG200_CASE_END
