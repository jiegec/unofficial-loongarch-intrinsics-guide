// atomic_v_tex_min_i32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_tex_min_i32, 2)
#include "atomic_v_tex_min_i32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_tex_min_i32)
CASE(old-smaller, 0xfffffff8, 0x3, 0xfffffff8, 0xfffffff8)
CASE(operand-smaller, 0x3, 0xfffffffc, 0x3, 0xfffffffc)
CASE(signed-bound, 0x7fffffff, 0x80000000, 0x7fffffff, 0x80000000)
LG200_CASE_END
