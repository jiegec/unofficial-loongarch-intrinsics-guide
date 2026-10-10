// v_fcvt_i32rpi_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_fcvt_i32rpi_f32, 1)
#include "v_fcvt_i32rpi_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_fcvt_i32rpi_f32)
CASE_FP(pos, rtne, 0x40200000, 0x3)
CASE_FP(neg, rtne, 0xc0200000, 0xfffffffe)
LG200_CASE_END
