// v_mov_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_mov_b32, 1)
#include "v_mov_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_mov_b32)
CASE(zero, 0x0, 0x0)
CASE(constant, 0xdeadbeef, 0xdeadbeef)
CASE(sign-bit, 0x80000000, 0x80000000)
LG200_CASE_END
