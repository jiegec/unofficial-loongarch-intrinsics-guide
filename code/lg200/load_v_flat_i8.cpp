// load_v_flat_i8 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_v_flat_i8, 2)
#include "load_v_flat_i8.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_v_flat_i8)
CASE(data-passthrough, 0x12345678, 0x0, 0x78)
CASE(zero-data, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x80000000, 0x0, 0x0)
CASE(byte-sign, 0x80, 0x0, 0xffffff80)
LG200_CASE_END
