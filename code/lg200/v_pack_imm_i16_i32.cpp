// v_pack_imm_i16_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_imm_i16_i32, 2)
#include "v_pack_imm_i16_i32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_imm_i16_i32)
CASE(basic, 0x8000, 0x7fff, 0x7fff7fff)
CASE(zero, 0x0, 0x0, 0x0)
CASE(field_order, 0x1234, 0x5678, 0x56781234)
CASE(imm_hi_positive, 0x0, 0x10000, 0x7fff0000)
CASE(imm_hi_negative, 0x0, 0xffff0000, 0x80000000)
CASE(imm_in_range_negative, 0x7fff, 0xffff8000, 0x80007fff)
LG200_CASE_END
