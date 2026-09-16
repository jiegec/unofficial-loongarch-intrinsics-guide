// gather_c_l -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(gather_c_l, 1)
#include "gather_c_l.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(gather_c_l)
CASE(data, 0x12345678, 0x12345678)
LG200_CASE_END
