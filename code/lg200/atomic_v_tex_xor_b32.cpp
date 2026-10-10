// atomic_v_tex_xor_b32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_tex_xor_b32, 2)
#include "atomic_v_tex_xor_b32.h"
LG200_ORACLE_END(2)

LG200_CASE_BEGIN(atomic_v_tex_xor_b32)
CASE(identity, 0x12345678, 0x0, 0x12345678, 0x12345678)
CASE(invert, 0xffffffff, 0xffffffff, 0xffffffff, 0x0)
CASE(toggle, 0xf0f0f0f, 0xf0f0f0f0, 0xf0f0f0f, 0xffffffff)
LG200_CASE_END
