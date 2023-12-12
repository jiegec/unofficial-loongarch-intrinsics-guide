#include "common.h"

v128 vaddwod_q_du_d(v128 a, v128 b) {
  v128 dst;
#include "vaddwod_q_du_d.h"
  return dst;
}

void test() { FUZZ2(vaddwod_q_du_d); }
