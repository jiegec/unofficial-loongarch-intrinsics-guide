#include "common.h"

v128 vmax_du(v128 a, v128 b) {
  v128 dst;
#include "vmax_du.h"
  return dst;
}

void test() { FUZZ2(vmax_du); }
