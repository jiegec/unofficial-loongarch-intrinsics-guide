#include "common.h"

v128 vsat_du(v128 a, int imm) {
  v128 dst;
#include "vsat_du.h"
  return dst;
}

void test() {
  FUZZ1(vsat_du, 0);
  FUZZ1(vsat_du, 7);
}
