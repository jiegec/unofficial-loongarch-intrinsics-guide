// gather_l_o -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(gather_l_o, 1)
#include "gather_l_o.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(gather_l_o)
CASE(data, 0x12345678, 0x12345678)
LG200_CASE_END
