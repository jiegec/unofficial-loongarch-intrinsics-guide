#include "common.h"

v128 vori_b(v128 a, int imm) {
  v128 dst;
#include "vori_b.h"
  return dst;
}

void test() {
  FUZZ1(vori_b, 0);
  FUZZ1(vori_b, 31);
}
