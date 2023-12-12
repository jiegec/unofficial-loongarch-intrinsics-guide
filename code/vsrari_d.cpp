#include "common.h"

v128 vsrari_d(v128 a, int imm) {
  v128 dst;
#include "vsrari_d.h"
  return dst;
}

void test() {
  FUZZ1(vsrari_d, 0);
  FUZZ1(vsrari_d, 7);
}
