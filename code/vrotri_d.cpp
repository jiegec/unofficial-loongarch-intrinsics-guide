#include "common.h"

v128 vrotri_d(v128 a, int imm) {
  v128 dst;
#include "vrotri_d.h"
  return dst;
}

void test() {
  FUZZ1(vrotri_d, 0);
  FUZZ1(vrotri_d, 7);
}
