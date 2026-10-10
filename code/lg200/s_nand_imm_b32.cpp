// s_nand_imm_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(s_nand_imm_b32, 2)
#include "s_nand_imm_b32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(s_nand_imm_b32)
CASE(imm-zero, 0x13579bdf, 0x0, 0xffffffff)
CASE(mask, 0x13579bdf, 0xf0f0f0f, 0xfcf8f4f0)
CASE(imm-all, 0x13579bdf, 0xffffffff, 0xeca86420)
LG200_CASE_END
