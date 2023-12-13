#include "common.h"

v256 xvbitclri_h(v256 a, int imm) {
  v256 dst;
#include "xvbitclri_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitclri_h, 0);
  XFUZZ1(xvbitclri_h, 3);
  XFUZZ1(xvbitclri_h, 7);
}
