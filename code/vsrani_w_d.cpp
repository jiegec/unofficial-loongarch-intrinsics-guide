#include "common.h"

v128 vsrani_w_d(v128 a, v128 b, int imm) {
  v128 dst;
#include "vsrani_w_d.h"
  return dst;
}

void test() {
  FUZZ2(vsrani_w_d, 0);
  FUZZ2(vsrani_w_d, 7);
}
