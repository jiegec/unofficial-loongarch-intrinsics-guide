// v_byteperm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_byteperm_b32, 3)
#include "v_byteperm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_byteperm_b32)
CASE(identity, 0x11223344, 0x55667788, 0x3020100, 0x11223344)
CASE(reversed, 0x11223344, 0x55667788, 0x7060504, 0x55667788)
CASE(zero-fill, 0x11223344, 0x55667788, 0x8090a0b, 0x0)
CASE(mixed, 0x11223344, 0x55667788, 0x6040a01, 0x66880033)
LG200_CASE_END
