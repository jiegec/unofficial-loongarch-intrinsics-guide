// v_xnor_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_xnor_b32, 2)
#include "v_xnor_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_xnor_b32)
CASE(same, 0xff, 0xff, 0xffffffff)
CASE(diff, 0xf, 0xf0, 0xffffff00)
LG200_CASE_END
