#include "common.h"

v128 vsrarn_w_d(v128 a, v128 b) {
  v128 dst;
#include "vsrarn_w_d.h"
  return dst;
}

void test() { FUZZ2(vsrarn_w_d); }
