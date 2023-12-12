#include "common.h"

v128 vsadd_b(v128 a, v128 b) {
  v128 dst;
#include "vsadd_b.h"
  return dst;
}

void test() { FUZZ2(vsadd_b); }
