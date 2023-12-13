#include "common.h"

v256 xvshuf4i_h(v256 a, int imm) {
  v256 dst;
#include "xvshuf4i_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvshuf4i_h, 0);
  XFUZZ1(xvshuf4i_h, 13);
  XFUZZ1(xvshuf4i_h, 100);
  XFUZZ1(xvshuf4i_h, 128);
  XFUZZ1(xvshuf4i_h, 255);
}
