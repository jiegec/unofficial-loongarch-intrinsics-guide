#include "common.h"

v256 xvsllwil_w_h(v256 a, int imm) {
  v256 dst;
#include "xvsllwil_w_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvsllwil_w_h, 0);
  XFUZZ1(xvsllwil_w_h, 7);
}
