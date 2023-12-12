#include "common.h"

v128 vbitrevi_d(v128 a, int imm) {
  v128 dst;
#include "vbitrevi_d.h"
  return dst;
}

void test() {
  FUZZ1(vbitrevi_d, 0);
  FUZZ1(vbitrevi_d, 3);
  FUZZ1(vbitrevi_d, 7);
}
