#include "common.h"

v256 xvbsll_v(v256 a, int imm) {
  v256 dst;
#include "xvbsll_v.h"
  return dst;
}

void test() {
  XFUZZ1(xvbsll_v, 0);
  XFUZZ1(xvbsll_v, 3);
}
