#include "common.h"

v128 vsat_wu(v128 a, int imm) {
  v128 dst;
#include "vsat_wu.h"
  return dst;
}

void test() {
  FUZZ1(vsat_wu, 0);
  FUZZ1(vsat_wu, 7);
}
