#include "common.h"

v128 vbitclri_h(v128 a, int imm) {
  v128 dst;
#include "vbitclri_h.h"
  return dst;
}

void test() {
  FUZZ1(vbitclri_h, 0);
  FUZZ1(vbitclri_h, 3);
  FUZZ1(vbitclri_h, 7);
}
