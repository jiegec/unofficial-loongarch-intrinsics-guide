#include "common.h"

v128 vrotri_b(v128 a, int imm) {
  v128 dst;
#include "vrotri_b.h"
  return dst;
}

void test() {
  FUZZ1(vrotri_b, 0);
  FUZZ1(vrotri_b, 7);
}
