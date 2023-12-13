#include "common.h"

v256 xvbitclri_w(v256 a, int imm) {
  v256 dst;
#include "xvbitclri_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitclri_w, 0);
  XFUZZ1(xvbitclri_w, 3);
  XFUZZ1(xvbitclri_w, 7);
}
