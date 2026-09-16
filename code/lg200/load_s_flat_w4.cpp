// load_s_flat_w4 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_s_flat_w4, 2)
#include "load_s_flat_w4.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(load_s_flat_w4)
CASE(data-passthrough, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678)
CASE(zero-data, 0x0, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x0, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
