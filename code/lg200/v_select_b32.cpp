// v_select_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_select_b32, 3)
#include "v_select_b32.h"
LG200_ORACLE_END(1)

// The condition is per lane and this oracle answers for lane 0, so a case whose
// condition has bit 0 set takes src0 and one with bit 0 clear takes src1.  The
// second case is what separates a bit test from a "nonzero means true" reading:
// 0x2 has bits set, but not in the lane under test.
LG200_CASE_BEGIN(v_select_b32)
// The word is the OR of the per-lane selections.  A condition whose low bits are
// all clear returns src1, one with every staged lane set returns src0, and any
// mixed condition returns src0|src1 - whatever the lane count is.  The vectors
// below are chosen so that both carriers agree: the PS carrier stages 16
// condition bits and the compute carrier 4, so a condition like 0xf is all-set on
// one and mixed on the other and cannot be a shared vector (see the row's desc).
CASE(condition-zero, 0x11111111, 0x22222222, 0x0, 0x22222222)
CASE(condition-low-bit, 0x11111111, 0x22222222, 0x1, 0x33333333)
CASE(condition-bit1, 0x11111111, 0x22222222, 0x2, 0x33333333)
CASE(condition-bits01, 0x11111111, 0x22222222, 0x3, 0x33333333)
CASE(condition-all16, 0x11111111, 0x22222222, 0xffff, 0x11111111)
CASE(condition-all32, 0x11111111, 0x22222222, 0xffffffff, 0x11111111)
CASE(condition-high-half-only, 0x11111111, 0x22222222, 0xffff0000, 0x22222222)
CASE(condition-high-bit-set, 0x11111111, 0x22222222, 0x10000, 0x22222222)
LG200_CASE_END
