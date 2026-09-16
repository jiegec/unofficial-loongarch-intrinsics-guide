// load_sm_matrix_x4 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_sm_matrix_x4, 2)
#include "load_sm_matrix_x4.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(load_sm_matrix_x4)
CASE(first-two-planes, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678)
CASE(zero-first-two, 0x0, 0x0, 0x0, 0x0)
LG200_CASE_END
