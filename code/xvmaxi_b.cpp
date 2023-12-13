#include "common.h"

v256 xvmaxi_b(v256 a, int imm) {
  v256 dst;
#include "xvmaxi_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvmaxi_b, 0);
  XFUZZ1(xvmaxi_b, 3);
  XFUZZ1(xvmaxi_b, 15);
}
