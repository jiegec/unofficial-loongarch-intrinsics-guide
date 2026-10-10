// gather -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(gather, 1)
#include "gather.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(gather)
CASE(data, 0x12345678, 0x12345678)
LG200_CASE_END
