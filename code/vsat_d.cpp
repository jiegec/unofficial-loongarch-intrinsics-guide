#include "common.h"

v128 vsat_d(v128 a, int imm) {
  v128 dst;
#include "vsat_d.h"
  return dst;
}

void test() {
  FUZZ1(vsat_d, 0);
  FUZZ1(vsat_d, 7);
}
