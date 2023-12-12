#include "common.h"

v128 vsrli_h(v128 a, int imm) {
  v128 dst;
#include "vsrli_h.h"
  return dst;
}

void test() {
  FUZZ1(vsrli_h, 0);
  FUZZ1(vsrli_h, 7);
}
