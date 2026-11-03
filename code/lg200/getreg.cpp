// getreg -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(getreg, 0)
#include "getreg.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(getreg)
CASE(env-register, 0x0)
LG200_CASE_END
