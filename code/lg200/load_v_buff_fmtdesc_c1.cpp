// load_v_buff_fmtdesc_c1 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_v_buff_fmtdesc_c1, 1)
#include "load_v_buff_fmtdesc_c1.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_v_buff_fmtdesc_c1)
CASE(uint-sign-bit, 0x8000, 0x8000)
CASE(uint-all-ones, 0xffff, 0xffff)
CASE(sint-sign-bit, 0xffff8000, 0xffff8000)
CASE(sint-all-ones, 0xffffffff, 0xffffffff)
CASE(first-oob, 0x0, 0x0)
LG200_CASE_END
