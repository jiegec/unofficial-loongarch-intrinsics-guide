#include "common.h"

v128 vmulwod_q_d(v128 a, v128 b) {
  v128 dst;
#include "vmulwod_q_d.h"
  return dst;
}

void test() { FUZZ2(vmulwod_q_d); }
