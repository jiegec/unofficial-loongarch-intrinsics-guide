// v_bfi_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bfi_b32, 3)
#include "v_bfi_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bfi_b32)
CASE(mask, 0x0, 0xf, 0xf0, 0xf0)
CASE(insert, 0xf, 0xf0, 0xf0, 0xf0)
LG200_CASE_END
