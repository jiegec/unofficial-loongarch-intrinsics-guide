#include "common.h"

v256 xvmaxi_w(v256 a, int imm) {
  v256 dst;
#include "xvmaxi_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvmaxi_w, 0);
  XFUZZ1(xvmaxi_w, 3);
  XFUZZ1(xvmaxi_w, 15);
}
