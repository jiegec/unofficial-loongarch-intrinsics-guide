#include "common.h"

v128 vsrari_b(v128 a, int imm) {
  v128 dst;
#include "vsrari_b.h"
  return dst;
}

void test() {
  FUZZ1(vsrari_b, 0);
  FUZZ1(vsrari_b, 7);
}
