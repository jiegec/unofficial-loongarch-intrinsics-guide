// barrier -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(barrier, 0)
#include "barrier.h"
LG200_ORACLE_END(0)

LG200_CASE_BEGIN(barrier)
CASE(control-effect)
LG200_CASE_END
