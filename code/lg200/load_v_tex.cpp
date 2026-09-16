// load_v_tex -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_v_tex, 2)
#include "load_v_tex.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_v_tex)
CASE(data-passthrough, 0x12345678, 0x0, 0x12345678)
CASE(zero-data, 0x0, 0x0, 0x0)
CASE(sign-bit-data, 0x80000000, 0x0, 0x80000000)
LG200_CASE_END
