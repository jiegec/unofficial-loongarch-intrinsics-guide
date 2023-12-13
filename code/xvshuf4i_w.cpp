#include "common.h"

v256 xvshuf4i_w(v256 a, int imm) {
  v256 dst;
#include "xvshuf4i_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvshuf4i_w, 0);
  XFUZZ1(xvshuf4i_w, 13);
  XFUZZ1(xvshuf4i_w, 100);
  XFUZZ1(xvshuf4i_w, 128);
  XFUZZ1(xvshuf4i_w, 255);
}
