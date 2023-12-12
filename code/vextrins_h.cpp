#include "common.h"

v128 vextrins_h(v128 a, v128 b, int imm) {
  v128 dst;
#include "vextrins_h.h"
  return dst;
}

void test() {
  FUZZ2(vextrins_h, 0);
  FUZZ2(vextrins_h, 3);
  FUZZ2(vextrins_h, 7);
  FUZZ2(vextrins_h, 15);
  FUZZ2(vextrins_h, 16);
  FUZZ2(vextrins_h, 32);
  FUZZ2(vextrins_h, 64);
  FUZZ2(vextrins_h, 128);
  FUZZ2(vextrins_h, 255);
}
