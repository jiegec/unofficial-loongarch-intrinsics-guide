// v_pack_snorm16_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_snorm16_f32, 1)
#include "v_pack_snorm16_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_snorm16_f32)
CASE_FP(zero, rtne, 0x0, 0x0, 0x0)
CASE_FP(one, rtne, 0x3f800000, 0x7fff)
CASE_FP(neg, rtne, 0xbf800000, 0x8001)
LG200_CASE_END
