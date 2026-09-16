// check -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(check, 0)
#include "check.h"
LG200_ORACLE_END(0)

LG200_CASE_BEGIN(check)
CASE(k-vm-vs-sm)
CASE(other-flags)
LG200_CASE_END
