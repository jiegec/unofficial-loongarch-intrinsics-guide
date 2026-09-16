// s_sra_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_sra_b32, 2)
#include "s_sra_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_sra_b32)
CASE(negative, 0x80000000, 0x1, 0xc0000000)
CASE(all-sign, 0xffffffff, 0x4, 0xffffffff)
LG200_CASE_END
