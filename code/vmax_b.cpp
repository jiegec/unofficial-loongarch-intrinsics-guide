#include "common.h"

v128 vmax_b(v128 a, v128 b) {
  v128 dst;
#include "vmax_b.h"
  return dst;
}

void test() { FUZZ2(vmax_b); }
