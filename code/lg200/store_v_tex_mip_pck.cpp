// store_v_tex_mip_pck -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(store_v_tex_mip_pck, 2)
#include "store_v_tex_mip_pck.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(store_v_tex_mip_pck)
CASE(data-passthrough, 0x12345678, 0x0, 0x12345678)
CASE(zero-data, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
