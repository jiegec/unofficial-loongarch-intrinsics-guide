#include "common.h"

v128 vsrarn_h_w(v128 a, v128 b) {
  v128 dst;
#include "vsrarn_h_w.h"
  return dst;
}

void test() { FUZZ2(vsrarn_h_w); }
