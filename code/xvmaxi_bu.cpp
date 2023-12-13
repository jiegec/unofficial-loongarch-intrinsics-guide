#include "common.h"

v256 xvmaxi_bu(v256 a, int imm) {
  v256 dst;
#include "xvmaxi_bu.h"
  return dst;
}

void test() {
  XFUZZ1(xvmaxi_bu, 0);
  XFUZZ1(xvmaxi_bu, 3);
  XFUZZ1(xvmaxi_bu, 15);
}
