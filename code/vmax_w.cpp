#include "common.h"

v128 vmax_w(v128 a, v128 b) {
  v128 dst;
#include "vmax_w.h"
  return dst;
}

void test() { FUZZ2(vmax_w); }
