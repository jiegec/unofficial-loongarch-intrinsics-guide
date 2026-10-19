// load_sm_i16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_sm_i16, 1)
#include "load_sm_i16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_sm_i16)
CASE(negative, 0xffffbeef, 0xffffbeef)
CASE(zero-data, 0x0, 0x0)
LG200_CASE_END
