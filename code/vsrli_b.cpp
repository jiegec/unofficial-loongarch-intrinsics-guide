#include "common.h"

v128 vsrli_b(v128 a, int imm) {
  v128 dst;
#include "vsrli_b.h"
  return dst;
}

void test() {
  FUZZ1(vsrli_b, 0);
  FUZZ1(vsrli_b, 7);
}
