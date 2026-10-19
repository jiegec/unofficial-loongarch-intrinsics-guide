// v_sra_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_sra_b32, 2)
#include "v_sra_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_sra_b32)
CASE(positive, 0x40000000, 0x1, 0x20000000)
CASE(negative, 0x80000000, 0x1, 0xc0000000)
CASE(all-sign, 0xffffffff, 0x4, 0xffffffff)
CASE(zero-shift, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
