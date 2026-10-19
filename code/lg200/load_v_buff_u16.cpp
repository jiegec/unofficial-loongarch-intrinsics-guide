// load_v_buff_u16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_v_buff_u16, 1)
#include "load_v_buff_u16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_v_buff_u16)
CASE(positive, 0x7fff, 0x7fff)
CASE(sign-bit, 0x8000, 0x8000)
CASE(all-ones, 0xffff, 0xffff)
CASE(first-oob, 0x0, 0x0)
LG200_CASE_END
