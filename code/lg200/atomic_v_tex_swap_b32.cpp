// atomic_v_tex_swap_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_tex_swap_b32, 2)
#include "atomic_v_tex_swap_b32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_tex_swap_b32)
CASE(basic, 0x11111111, 0xaabbccdd, 0x11111111, 0xaabbccdd)
CASE(zero-src, 0x12345678, 0x0, 0x12345678, 0x0)
CASE(zero-old, 0x0, 0xffffffff, 0x0, 0xffffffff)
CASE(self, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef)
LG200_CASE_END
