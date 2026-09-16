// load_v_flat_i16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_v_flat_i16, 2)
#include "load_v_flat_i16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_v_flat_i16)
CASE(data-passthrough, 0x12345678, 0x0, 0x12345678)
CASE(zero-data, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
