#include "common.h"

v128 vmulwev_w_h(v128 a, v128 b) {
  v128 dst;
#include "vmulwev_w_h.h"
  return dst;
}

void test() { FUZZ2(vmulwev_w_h); }
