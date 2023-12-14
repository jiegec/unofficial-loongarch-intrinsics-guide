#include "common.h"

v256 xvbsrl_v(v256 a, int imm) {
  v256 dst;
#include "xvbsrl_v.h"
  return dst;
}

void test() {
  XFUZZ1(xvbsrl_v, 0);
  XFUZZ1(xvbsrl_v, 3);
}
