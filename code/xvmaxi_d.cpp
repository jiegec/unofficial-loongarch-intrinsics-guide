#include "common.h"

v256 xvmaxi_d(v256 a, int imm) {
  v256 dst;
#include "xvmaxi_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvmaxi_d, 0);
  XFUZZ1(xvmaxi_d, 3);
  XFUZZ1(xvmaxi_d, 15);
}
