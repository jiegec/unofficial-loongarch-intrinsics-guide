// trap -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(trap, 0)
#include "trap.h"
LG200_ORACLE_END(0)

LG200_CASE_BEGIN(trap)
CASE(control-effect)
LG200_CASE_END
