#include "common.h"

v128 vsub_b(v128 a, v128 b) {
  v128 dst;
#include "vsub_b.h"
  return dst;
}

void test() { FUZZ2(vsub_b); }
