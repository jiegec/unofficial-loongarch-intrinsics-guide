#include "common.h"

v256 xvaddi_wu(v256 a, int imm) {
  v256 dst;
#include "xvaddi_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvaddi_wu, 0);
  XFUZZ1(xvaddi_wu, 31);
}
