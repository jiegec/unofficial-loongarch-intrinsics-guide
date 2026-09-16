// atomic_v_tex_max_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_tex_max_i32, 2)
#include "atomic_v_tex_max_i32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_tex_max_i32)
CASE(old-larger, 0xfffffffc, 0xfffffffb, 0xfffffffc, 0xfffffffc)
CASE(operand-larger, 0xfffffffc, 0x3, 0xfffffffc, 0x3)
CASE(signed-bound, 0x80000000, 0x7fffffff, 0x80000000, 0x7fffffff)
LG200_CASE_END
