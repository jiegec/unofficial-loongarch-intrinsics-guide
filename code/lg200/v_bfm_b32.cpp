// v_bfm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_bfm_b32, 2)
#include "v_bfm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_bfm_b32)
CASE(zero-offset, 0xffff, 0x0, 0x7fffffff)
CASE(shift, 0xf, 0x4, 0x7fff0)
CASE(top, 0xf, 0x1c, 0xf0000000)
CASE(wrap, 0xf, 0x24, 0x7fff0)
LG200_CASE_END
