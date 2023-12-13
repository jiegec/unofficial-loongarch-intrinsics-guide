#include "common.h"

v128 vsat_bu(v128 a, int imm) {
  v128 dst;
#include "vsat_bu.h"
  return dst;
}

void test() {
  FUZZ1(vsat_bu, 0);
  FUZZ1(vsat_bu, 7);
}
