#include "common.h"

v128 vbitclri_w(v128 a, int imm) {
  v128 dst;
#include "vbitclri_w.h"
  return dst;
}

void test() {
  FUZZ1(vbitclri_w, 0);
  FUZZ1(vbitclri_w, 3);
  FUZZ1(vbitclri_w, 7);
}
