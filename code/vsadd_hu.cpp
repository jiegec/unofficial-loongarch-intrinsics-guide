#include "common.h"

v128 vsadd_hu(v128 a, v128 b) {
  v128 dst;
#include "vsadd_hu.h"
  return dst;
}

void test() { FUZZ2(vsadd_hu); }
