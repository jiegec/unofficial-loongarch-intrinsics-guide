#include "common.h"

v128 vsadd_d(v128 a, v128 b) {
  v128 dst;
#include "vsadd_d.h"
  return dst;
}

void test() { FUZZ2(vsadd_d); }
