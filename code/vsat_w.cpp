#include "common.h"

v128 vsat_w(v128 a, int imm) {
  v128 dst;
#include "vsat_w.h"
  return dst;
}

void test() {
  FUZZ1(vsat_w, 0);
  FUZZ1(vsat_w, 7);
}
