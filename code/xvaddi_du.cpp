#include "common.h"

v256 xvaddi_du(v256 a, int imm) {
  v256 dst;
#include "xvaddi_du.h"
  return dst;
}

void test() {
  XFUZZ1(xvaddi_du, 0);
  XFUZZ1(xvaddi_du, 31);
}
