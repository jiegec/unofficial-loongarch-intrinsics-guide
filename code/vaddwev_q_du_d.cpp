#include "common.h"

v128 vaddwev_q_du_d(v128 a, v128 b) {
  v128 dst;
#include "vaddwev_q_du_d.h"
  return dst;
}

void test() { FUZZ2(vaddwev_q_du_d); }
