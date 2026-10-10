// load_sm_matrix_x2 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_sm_matrix_x2, 2)
#include "load_sm_matrix_x2.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(load_sm_matrix_x2)
CASE(two-planes, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678)
CASE(zero-planes, 0x0, 0x0, 0x0, 0x0)
LG200_CASE_END
