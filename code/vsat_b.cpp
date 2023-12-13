#include "common.h"

v128 vsat_b(v128 a, int imm) {
  v128 dst;
#include "vsat_b.h"
  return dst;
}

void test() {
  FUZZ1(vsat_b, 0);
  FUZZ1(vsat_b, 7);
}
