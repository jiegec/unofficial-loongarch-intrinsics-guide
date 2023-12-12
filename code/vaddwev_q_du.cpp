#include "common.h"

v128 vaddwev_q_du(v128 a, v128 b) {
  v128 dst;
#include "vaddwev_q_du.h"
  return dst;
}

void test() { FUZZ2(vaddwev_q_du); }
