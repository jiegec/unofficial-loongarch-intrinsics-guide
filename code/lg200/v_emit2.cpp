// v_emit2 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_emit2, 3)
#include "v_emit2.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_emit2)
CASE(data, 0x12345678, 0x0, 0x0, 0x12345678)
LG200_CASE_END
