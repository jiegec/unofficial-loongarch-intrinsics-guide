#include "common.h"

v128 vsrani_b_h(v128 a, v128 b, int imm) {
  v128 dst;
#include "vsrani_b_h.h"
  return dst;
}

void test() {
  FUZZ2(vsrani_b_h, 0);
  FUZZ2(vsrani_b_h, 7);
  FUZZ2(vsrani_b_h, 15);
}
