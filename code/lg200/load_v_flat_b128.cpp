// load_v_flat_b128 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_v_flat_b128, 4)
#include "load_v_flat_b128.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(load_v_flat_b128)
CASE(data-passthrough, 0x9abcdef0, 0x12345678, 0x55555555, 0xdeadbeef, 0x9abcdef0, 0x12345678, 0x55555555, 0xdeadbeef)
CASE(zero-data, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x0, 0x80000000, 0x80000000, 0x80000000, 0x0, 0x80000000, 0x80000000, 0x80000000)
LG200_CASE_END
