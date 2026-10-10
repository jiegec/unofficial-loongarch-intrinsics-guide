// v_frexp_exp_i32_f64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_frexp_exp_i32_f64, 2)
#include "v_frexp_exp_i32_f64.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_frexp_exp_i32_f64)
CASE_FP(one, rtne, 0x0, 0x3ff00000, 0x1)
LG200_CASE_END
