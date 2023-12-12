#include "common.h"

v128 vsubwod_q_du(v128 a, v128 b) {
  v128 dst;
#include "vsubwod_q_du.h"
  return dst;
}

void test() { FUZZ2(vsubwod_q_du); }
