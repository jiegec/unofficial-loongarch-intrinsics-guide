#include "common.h"

v256 xvshuf4i_b(v256 a, int imm) {
  v256 dst;
#include "xvshuf4i_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvshuf4i_b, 0);
  XFUZZ1(xvshuf4i_b, 13);
  XFUZZ1(xvshuf4i_b, 100);
  XFUZZ1(xvshuf4i_b, 128);
  XFUZZ1(xvshuf4i_b, 255);
}
