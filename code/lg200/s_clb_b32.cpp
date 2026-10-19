// s_clb_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_clb_b32, 1)
#include "s_clb_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_clb_b32)
CASE(positive, 0xffff, 0x10)
CASE(negative, 0xffff0000, 0x10)
LG200_CASE_END
