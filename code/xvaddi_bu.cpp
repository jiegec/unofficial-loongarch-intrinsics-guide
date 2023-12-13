#include "common.h"

v256 xvaddi_bu(v256 a, int imm) {
  v256 dst;
#include "xvaddi_bu.h"
  return dst;
}

void test() {
  XFUZZ1(xvaddi_bu, 0);
  XFUZZ1(xvaddi_bu, 31);
}
