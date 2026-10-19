// v_cmpclass_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_cmpclass_f32, 2)
#include "v_cmpclass_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_cmpclass_f32)
CASE(pos-normal, 0x3f800000, 0x100, 0xffff)
CASE(qnan, 0x7fc00000, 0x2, 0xffff)
CASE(snan-only, 0x7f800001, 0x2, 0x0)
CASE(norm-mask, 0x3f800000, 0x108, 0xffff)
CASE(sub-mask, 0x80000, 0x108, 0x0)
CASE(finite-nan, 0x7fc00000, 0x1f8, 0x0)
CASE(finite-inf, 0x7f800000, 0x1f8, 0x0)
CASE(finite-norm, 0x3f800000, 0x1f8, 0xffff)
LG200_CASE_END
