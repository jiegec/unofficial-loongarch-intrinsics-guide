#include "common.h"

v128 vsrli_d(v128 a, int imm) {
  v128 dst;
#include "vsrli_d.h"
  return dst;
}

void test() {
  FUZZ1(vsrli_d, 0);
  FUZZ1(vsrli_d, 7);
}
