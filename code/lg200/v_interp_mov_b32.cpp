// v_interp_mov_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_interp_mov_b32, 2)
#include "v_interp_mov_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_interp_mov_b32)
CASE(data, 0x12345678, 0x0, 0x12345678)
LG200_CASE_END
