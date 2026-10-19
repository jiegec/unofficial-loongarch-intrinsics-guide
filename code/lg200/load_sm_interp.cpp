// load_sm_interp -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_sm_interp, 2)
#include "load_sm_interp.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_sm_interp)
CASE(data, 0x12345678, 0x0, 0x12345678)
LG200_CASE_END
