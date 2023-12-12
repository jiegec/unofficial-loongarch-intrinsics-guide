#include "common.h"

v128 vbitrevi_w(v128 a, int imm) {
  v128 dst;
#include "vbitrevi_w.h"
  return dst;
}

void test() {
  FUZZ1(vbitrevi_w, 0);
  FUZZ1(vbitrevi_w, 3);
  FUZZ1(vbitrevi_w, 7);
}
