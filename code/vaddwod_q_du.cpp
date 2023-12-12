#include "common.h"

v128 vaddwod_q_du(v128 a, v128 b) {
  v128 dst;
#include "vaddwod_q_du.h"
  return dst;
}

void test() { FUZZ2(vaddwod_q_du); }
