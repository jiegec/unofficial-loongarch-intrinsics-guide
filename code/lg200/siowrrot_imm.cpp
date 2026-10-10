// siowrrot_imm -- CPU oracle and case vectors.
#include "ops/oracle_macros.hpp"

LG200_ORACLE_BEGIN(siowrrot_imm, 0)
#include "siowrrot_imm.h"
LG200_ORACLE_END(0)

LG200_CASE_BEGIN(siowrrot_imm)
CASE(sio1-write)
CASE(sio0-write)
LG200_CASE_END
