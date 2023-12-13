#include "common.h"

v256 xvmaxi_du(v256 a, int imm) {
  v256 dst;
#include "xvmaxi_du.h"
  return dst;
}

void test() {
  XFUZZ1(xvmaxi_du, 0);
  XFUZZ1(xvmaxi_du, 3);
  XFUZZ1(xvmaxi_du, 15);
}
