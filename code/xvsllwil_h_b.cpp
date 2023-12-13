#include "common.h"

v256 xvsllwil_h_b(v256 a, int imm) {
  v256 dst;
#include "xvsllwil_h_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvsllwil_h_b, 0);
  XFUZZ1(xvsllwil_h_b, 7);
}
