// v_firstv2s_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_firstv2s_b32, 1)
#include "v_firstv2s_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_firstv2s_b32)
CASE(data, 0x12345678, 0x12345678)
CASE(low16, 0xffff, 0xffff)
LG200_CASE_END
