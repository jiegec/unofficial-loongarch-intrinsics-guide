#include "common.h"

v128 vsub_d(v128 a, v128 b) {
  v128 dst;
#include "vsub_d.h"
  return dst;
}

void test() { FUZZ2(vsub_d); }
