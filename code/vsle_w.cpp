#include "common.h"

v128 vsle_w(v128 a, v128 b) {
  v128 dst;
#include "vsle_w.h"
  return dst;
}

void test() { FUZZ2(vsle_w); }
