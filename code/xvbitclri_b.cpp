#include "common.h"

v256 xvbitclri_b(v256 a, int imm) {
  v256 dst;
#include "xvbitclri_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitclri_b, 0);
  XFUZZ1(xvbitclri_b, 3);
  XFUZZ1(xvbitclri_b, 7);
}
