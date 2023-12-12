#include "common.h"

v128 vhaddw_q_d(v128 a, v128 b) {
  v128 dst;
#include "vhaddw_q_d.h"
  return dst;
}

void test() { FUZZ2(vhaddw_q_d); }
