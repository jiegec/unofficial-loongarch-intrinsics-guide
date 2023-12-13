#include "common.h"

v256 xvaddi_hu(v256 a, int imm) {
  v256 dst;
#include "xvaddi_hu.h"
  return dst;
}

void test() {
  XFUZZ1(xvaddi_hu, 0);
  XFUZZ1(xvaddi_hu, 31);
}
