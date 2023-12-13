#include "common.h"

v128 vsat_hu(v128 a, int imm) {
  v128 dst;
#include "vsat_hu.h"
  return dst;
}

void test() {
  FUZZ1(vsat_hu, 0);
  FUZZ1(vsat_hu, 7);
}
