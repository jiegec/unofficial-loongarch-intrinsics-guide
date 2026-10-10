// nop -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(nop, 0)
#include "nop.h"
LG200_ORACLE_END(0)

LG200_CASE_BEGIN(nop)
CASE(no-effect)
CASE(filler)
LG200_CASE_END
