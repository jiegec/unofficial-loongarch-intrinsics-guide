#include "common.h"

v256 xvbitseti_d(v256 a, int imm) {
  v256 dst;
#include "xvbitseti_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitseti_d, 0);
  XFUZZ1(xvbitseti_d, 3);
  XFUZZ1(xvbitseti_d, 7);
}
