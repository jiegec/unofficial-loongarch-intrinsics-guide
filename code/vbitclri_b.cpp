#include "common.h"

v128 vbitclri_b(v128 a, int imm) {
  v128 dst;
#include "vbitclri_b.h"
  return dst;
}

void test() {
  FUZZ1(vbitclri_b, 0);
  FUZZ1(vbitclri_b, 3);
  FUZZ1(vbitclri_b, 7);
}
