#include "common.h"

v128 vbitrevi_h(v128 a, int imm) {
  v128 dst;
#include "vbitrevi_h.h"
  return dst;
}

void test() {
  FUZZ1(vbitrevi_h, 0);
  FUZZ1(vbitrevi_h, 3);
  FUZZ1(vbitrevi_h, 7);
}
