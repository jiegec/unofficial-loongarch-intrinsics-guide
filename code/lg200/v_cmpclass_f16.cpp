// v_cmpclass_f16 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmpclass_f16, 2)
#include "v_cmpclass_f16.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmpclass_f16)
CASE(pos-normal, 0x3c00, 0x100, 0xffff)
CASE(qnan, 0x7e00, 0x2, 0xffff)
CASE(pos-inf, 0x7c00, 0x200, 0xffff)
LG200_CASE_END
