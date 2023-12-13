#include "common.h"

v256 xvmaxi_h(v256 a, int imm) {
  v256 dst;
#include "xvmaxi_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvmaxi_h, 0);
  XFUZZ1(xvmaxi_h, 3);
  XFUZZ1(xvmaxi_h, 15);
}
