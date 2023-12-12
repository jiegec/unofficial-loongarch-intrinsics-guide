#include "common.h"

v128 vmulwod_d_wu_w(v128 a, v128 b) {
  v128 dst;
#include "vmulwod_d_wu_w.h"
  return dst;
}

void test() { FUZZ2(vmulwod_d_wu_w); }
