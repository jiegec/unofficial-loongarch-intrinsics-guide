#include "common.h"

v128 vmulwev_h_b(v128 a, v128 b) {
  v128 dst;
#include "vmulwev_h_b.h"
  return dst;
}

void test() { FUZZ2(vmulwev_h_b); }
