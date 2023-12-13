#include "common.h"

v256 xvbitseti_h(v256 a, int imm) {
  v256 dst;
#include "xvbitseti_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitseti_h, 0);
  XFUZZ1(xvbitseti_h, 3);
  XFUZZ1(xvbitseti_h, 7);
}
