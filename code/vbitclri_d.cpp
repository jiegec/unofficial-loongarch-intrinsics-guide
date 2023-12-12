#include "common.h"

v128 vbitclri_d(v128 a, int imm) {
  v128 dst;
#include "vbitclri_d.h"
  return dst;
}

void test() {
  FUZZ1(vbitclri_d, 0);
  FUZZ1(vbitclri_d, 3);
  FUZZ1(vbitclri_d, 7);
}
