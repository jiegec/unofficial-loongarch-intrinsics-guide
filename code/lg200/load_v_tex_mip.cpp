// load_v_tex_mip -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(load_v_tex_mip, 1)
#include "load_v_tex_mip.h"
LG200_ORACLE_END(1)

LG200_CASE_BEGIN(load_v_tex_mip)
CASE(data, 0x12345678, 0x12345678)
// texel x-address mapping: row i fetches texel i at (x=i, y=0, lod=0); with
// a distinct value per row, a PASS proves the pixel-center x addressing.
CASE(x1, 0x10000001, 0x10000001)
CASE(x2, 0x10000002, 0x10000002)
CASE(x3, 0x10000003, 0x10000003)
CASE(x4, 0x10000004, 0x10000004)
CASE(x5, 0x10000005, 0x10000005)
CASE(x6, 0x10000006, 0x10000006)
CASE(x7, 0x10000007, 0x10000007)
LG200_CASE_END
