// v_emit3 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_emit3, 3)
#include "v_emit3.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_emit3)
CASE(data, 0x12345678, 0x0, 0x0, 0x12345678)
LG200_CASE_END
