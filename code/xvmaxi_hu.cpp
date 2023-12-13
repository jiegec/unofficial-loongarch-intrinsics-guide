#include "common.h"

v256 xvmaxi_hu(v256 a, int imm) {
  v256 dst;
#include "xvmaxi_hu.h"
  return dst;
}

void test() {
  XFUZZ1(xvmaxi_hu, 0);
  XFUZZ1(xvmaxi_hu, 3);
  XFUZZ1(xvmaxi_hu, 15);
}
