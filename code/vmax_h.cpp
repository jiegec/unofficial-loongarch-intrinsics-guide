#include "common.h"

v128 vmax_h(v128 a, v128 b) {
  v128 dst;
#include "vmax_h.h"
  return dst;
}

void test() { FUZZ2(vmax_h); }
