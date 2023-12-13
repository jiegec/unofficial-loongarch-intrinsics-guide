#include "common.h"

v256 xvbitseti_b(v256 a, int imm) {
  v256 dst;
#include "xvbitseti_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitseti_b, 0);
  XFUZZ1(xvbitseti_b, 3);
  XFUZZ1(xvbitseti_b, 7);
}
