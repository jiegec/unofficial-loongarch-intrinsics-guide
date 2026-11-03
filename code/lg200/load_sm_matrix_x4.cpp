// load_sm_matrix_x4 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_sm_matrix_x4, 4)
#include "load_sm_matrix_x4.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(load_sm_matrix_x4)
CASE(four-planes, 0x9abcdef0, 0x12345678, 0xdeadbeef, 0x0badf00d, 0x9abcdef0, 0x12345678, 0xdeadbeef, 0x0badf00d)
CASE(zero-planes, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
LG200_CASE_END
