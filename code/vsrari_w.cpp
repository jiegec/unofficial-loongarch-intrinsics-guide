#include "common.h"

v128 vsrari_w(v128 a, int imm) {
  v128 dst;
#include "vsrari_w.h"
  return dst;
}

void test() {
  FUZZ1(vsrari_w, 0);
  FUZZ1(vsrari_w, 7);
}
