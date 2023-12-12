#include "common.h"

v128 vsub_h(v128 a, v128 b) {
  v128 dst;
#include "vsub_h.h"
  return dst;
}

void test() { FUZZ2(vsub_h); }
