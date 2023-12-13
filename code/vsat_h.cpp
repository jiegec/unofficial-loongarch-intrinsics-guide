#include "common.h"

v128 vsat_h(v128 a, int imm) {
  v128 dst;
#include "vsat_h.h"
  return dst;
}

void test() {
  FUZZ1(vsat_h, 0);
  FUZZ1(vsat_h, 7);
}
