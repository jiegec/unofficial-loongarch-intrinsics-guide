// getpc -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(getpc, 0)
#include "getpc.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(getpc)
CASE(control-position, 0x0, 0x0)
LG200_CASE_END
