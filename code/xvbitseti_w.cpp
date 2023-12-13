#include "common.h"

v256 xvbitseti_w(v256 a, int imm) {
  v256 dst;
#include "xvbitseti_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvbitseti_w, 0);
  XFUZZ1(xvbitseti_w, 3);
  XFUZZ1(xvbitseti_w, 7);
}
