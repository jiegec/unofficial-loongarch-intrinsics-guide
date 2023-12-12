#include "common.h"

v128 vsadd_h(v128 a, v128 b) {
  v128 dst;
#include "vsadd_h.h"
  return dst;
}

void test() { FUZZ2(vsadd_h); }
