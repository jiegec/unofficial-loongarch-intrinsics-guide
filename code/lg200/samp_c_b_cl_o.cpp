// samp_c_b_cl_o -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(samp_c_b_cl_o, 2)
#include "samp_c_b_cl_o.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(samp_c_b_cl_o)
CASE(data-passthrough, 0x12345678, 0x0, 0x12345678)
CASE(zero-data, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
