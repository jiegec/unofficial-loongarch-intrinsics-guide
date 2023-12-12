#include "common.h"

v128 vaddwod_w_h(v128 a, v128 b) {
  v128 dst;
#include "vaddwod_w_h.h"
  return dst;
}

void test() { FUZZ2(vaddwod_w_h); }
