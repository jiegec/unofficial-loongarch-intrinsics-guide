#include "common.h"

v128 vmulwev_d_w(v128 a, v128 b) {
  v128 dst;
#include "vmulwev_d_w.h"
  return dst;
}

void test() { FUZZ2(vmulwev_d_w); }
