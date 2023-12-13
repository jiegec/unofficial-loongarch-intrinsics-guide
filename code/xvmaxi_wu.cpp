#include "common.h"

v256 xvmaxi_wu(v256 a, int imm) {
  v256 dst;
#include "xvmaxi_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvmaxi_wu, 0);
  XFUZZ1(xvmaxi_wu, 3);
  XFUZZ1(xvmaxi_wu, 15);
}
