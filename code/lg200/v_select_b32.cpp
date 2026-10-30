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
CASE(lane0-set, 0x11111111, 0x22222222, 0x1, 0x11111111)
CASE(lane0-clear, 0x11111111, 0x22222222, 0x2, 0x22222222)
CASE(condition-zero, 0x11111111, 0x22222222, 0x0, 0x22222222)
CASE(condition-all, 0x11111111, 0x22222222, 0xffffffff, 0x11111111)
LG200_CASE_END
