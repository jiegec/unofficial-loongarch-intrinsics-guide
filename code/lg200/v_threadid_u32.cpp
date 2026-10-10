// v_threadid_u32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_threadid_u32, 1)
#include "v_threadid_u32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_threadid_u32)
CASE(lane0, 0x0, 0x0)
LG200_CASE_END
