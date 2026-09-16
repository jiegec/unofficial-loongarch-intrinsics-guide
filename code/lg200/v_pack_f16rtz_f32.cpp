// v_pack_f16rtz_f32 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(v_pack_f16rtz_f32, 2)
#include "v_pack_f16rtz_f32.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(v_pack_f16rtz_f32)
CASE(zero-zero, 0x0, 0x0, 0x0)
CASE(one-one, 0x3f800000, 0x3f800000, 0x3c003c00)
CASE(one-two, 0x3f800000, 0x40000000, 0x40003c00)
CASE(half-minus-one, 0x3f000000, 0xbf800000, 0xbc003800)
CASE(small-rtz, 0x3c8efa35, 0x3c8efa35, 0x24772477)
LG200_CASE_END
