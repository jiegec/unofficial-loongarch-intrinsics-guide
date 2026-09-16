// load_sm_u16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_sm_u16, 1)
#include "load_sm_u16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_sm_u16)
CASE(sign-bit, 0xbeef, 0xbeef)
CASE(zero-data, 0x0, 0x0)
LG200_CASE_END
