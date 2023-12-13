#include "common.h"

v256 xvsllwil_d_w(v256 a, int imm) {
  v256 dst;
#include "xvsllwil_d_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvsllwil_d_w, 0);
  XFUZZ1(xvsllwil_d_w, 7);
}
