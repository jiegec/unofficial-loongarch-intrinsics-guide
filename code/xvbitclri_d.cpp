#include "common.h"

v256 xvbitclri_d(v256 a, int imm) {
  v256 dst;
#include "xvbitclri_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitclri_d, 0);
  XFUZZ1(xvbitclri_d, 3);
  XFUZZ1(xvbitclri_d, 7);
}
