// load_sm_matrix_x1 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_sm_matrix_x1, 1)
#include "load_sm_matrix_x1.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_sm_matrix_x1)
CASE(data-passthrough, 0x12345678, 0x12345678)
CASE(zero-data, 0x0, 0x0)
LG200_CASE_END
