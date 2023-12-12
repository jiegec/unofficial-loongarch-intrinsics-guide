#include "common.h"

v128 vextrins_b(v128 a, v128 b, int imm) {
  v128 dst;
#include "vextrins_b.h"
  return dst;
}

void test() {
  FUZZ2(vextrins_b, 0);
  FUZZ2(vextrins_b, 3);
  FUZZ2(vextrins_b, 7);
  FUZZ2(vextrins_b, 15);
  FUZZ2(vextrins_b, 16);
  FUZZ2(vextrins_b, 32);
  FUZZ2(vextrins_b, 64);
  FUZZ2(vextrins_b, 128);
  FUZZ2(vextrins_b, 255);
}
