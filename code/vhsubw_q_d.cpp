#include "common.h"

v128 vhsubw_q_d(v128 a, v128 b) {
  v128 dst;
#include "vhsubw_q_d.h"
  return dst;
}

void test() { FUZZ2(vhsubw_q_d); }
