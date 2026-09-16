// atomic_v_buff_cmpswap_b64 -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(atomic_v_buff_cmpswap_b64, 6)
#include "atomic_v_buff_cmpswap_b64.h"
LG200_ORACLE_END(4)

LG200_CASE_BEGIN(atomic_v_buff_cmpswap_b64)
CASE(match, 0x55667788, 0x11223344, 0x55667788, 0x11223344, 0x0, 0xffffffff, 0x55667788, 0x11223344, 0x0, 0xffffffff)
CASE(mismatch, 0x55667788, 0x11223344, 0x55667789, 0x11223344, 0xcafebabe, 0xdeadbeef, 0x55667788, 0x11223344, 0x55667788, 0x11223344)
LG200_CASE_END
