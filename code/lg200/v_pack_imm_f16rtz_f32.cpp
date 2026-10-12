// v_pack_imm_f16rtz_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_imm_f16rtz_f32, 2)
#include "v_pack_imm_f16rtz_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_imm_f16rtz_f32)
CASE_FP(one, rtne, 0x3f800000, 0x0, 0x3c00)
CASE_FP(nan-pos, rtne, 0x7fc00000 0x3f800000, 0x00007bff)
CASE_FP(nan-neg, rtne, 0xffc00000 0x3f800000, 0x80007bff)
CASE_FP(inf-pos, rtne, 0x7f800000 0x3f800000, 0x00007bff)
CASE_FP(inf-neg, rtne, 0xff800000 0x3f800000, 0x80007bff)
CASE_FP(zero-neg, rtne, 0x80000000 0x3f800000, 0x00008000)
CASE_FP(subnormal-min, rtne, 0x1 0x3f800000, 0x00000000)
CASE_FP(subnormal-neg, rtne, 0x807fffff 0x3f800000, 0x00008000)
LG200_CASE_END
