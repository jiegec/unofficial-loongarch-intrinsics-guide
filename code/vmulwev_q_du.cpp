#include "common.h"

v128 vmulwev_q_du(v128 a, v128 b) {
  v128 dst;
#include "vmulwev_q_du.h"
  return dst;
}

void test() { FUZZ2(vmulwev_q_du); }
