// load_sm_i8 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_sm_i8, 1)
#include "load_sm_i8.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_sm_i8)
CASE(negative, 0xffffffaa, 0xffffffaa)
CASE(zero-data, 0x0, 0x0)
LG200_CASE_END
