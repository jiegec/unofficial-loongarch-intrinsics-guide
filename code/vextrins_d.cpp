#include "common.h"

v128 vextrins_d(v128 a, v128 b, int imm) {
  v128 dst;
#include "vextrins_d.h"
  return dst;
}

void test() {
  FUZZ2(vextrins_d, 0);
  FUZZ2(vextrins_d, 3);
  FUZZ2(vextrins_d, 7);
  FUZZ2(vextrins_d, 15);
  FUZZ2(vextrins_d, 16);
  FUZZ2(vextrins_d, 32);
  FUZZ2(vextrins_d, 64);
  FUZZ2(vextrins_d, 128);
  FUZZ2(vextrins_d, 255);
}
