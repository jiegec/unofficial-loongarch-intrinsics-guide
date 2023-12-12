#include "common.h"

v128 vsadd_du(v128 a, v128 b) {
  v128 dst;
#include "vsadd_du.h"
  return dst;
}

void test() { FUZZ2(vsadd_du); }
