#include "common.h"

v128 vsub_w(v128 a, v128 b) {
  v128 dst;
#include "vsub_w.h"
  return dst;
}

void test() { FUZZ2(vsub_w); }
