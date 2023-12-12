#include "common.h"

v128 vextrins_w(v128 a, v128 b, int imm) {
  v128 dst;
#include "vextrins_w.h"
  return dst;
}

void test() {
  FUZZ2(vextrins_w, 0);
  FUZZ2(vextrins_w, 3);
  FUZZ2(vextrins_w, 7);
  FUZZ2(vextrins_w, 15);
  FUZZ2(vextrins_w, 16);
  FUZZ2(vextrins_w, 32);
  FUZZ2(vextrins_w, 64);
  FUZZ2(vextrins_w, 128);
  FUZZ2(vextrins_w, 255);
}
