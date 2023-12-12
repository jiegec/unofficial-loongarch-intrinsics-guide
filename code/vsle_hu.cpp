#include "common.h"

v128 vsle_hu(v128 a, v128 b) {
  v128 dst;
#include "vsle_hu.h"
  return dst;
}

void test() { FUZZ2(vsle_hu); }
