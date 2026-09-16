// v_fcvt_f32off_i4 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_f32off_i4, 1)
#include "v_fcvt_f32off_i4.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_f32off_i4)
CASE(data, 0x3f800000, 0x0)
LG200_CASE_END
