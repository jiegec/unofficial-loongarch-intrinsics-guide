#include "common.h"

v128 vaddwev_w_h(v128 a, v128 b) {
  v128 dst;
#include "vaddwev_w_h.h"
  return dst;
}

void test() { FUZZ2(vaddwev_w_h); }
