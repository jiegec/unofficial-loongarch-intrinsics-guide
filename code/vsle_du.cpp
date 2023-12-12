#include "common.h"

v128 vsle_du(v128 a, v128 b) {
  v128 dst;
#include "vsle_du.h"
  return dst;
}

void test() { FUZZ2(vsle_du); }
