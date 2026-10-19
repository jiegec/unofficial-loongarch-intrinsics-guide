// exit -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(exit, 0)
#include "exit.h"
LG200_ORACLE_END(0)

LG200_CASE_BEGIN(exit)
CASE(terminates)
LG200_CASE_END
