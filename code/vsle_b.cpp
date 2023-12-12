#include "common.h"

v128 vsle_b(v128 a, v128 b) {
  v128 dst;
#include "vsle_b.h"
  return dst;
}

void test() { FUZZ2(vsle_b); }
