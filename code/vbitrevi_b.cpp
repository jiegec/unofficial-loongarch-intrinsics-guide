#include "common.h"

v128 vbitrevi_b(v128 a, int imm) {
  v128 dst;
#include "vbitrevi_b.h"
  return dst;
}

void test() {
  FUZZ1(vbitrevi_b, 0);
  FUZZ1(vbitrevi_b, 3);
  FUZZ1(vbitrevi_b, 7);
}
