// getlod -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(getlod, 1)
#include "getlod.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(getlod)
CASE(data, 0x12345678, 0x12345678)
LG200_CASE_END
