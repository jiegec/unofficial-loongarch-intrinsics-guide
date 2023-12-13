#include "common.h"

v256 xvslti_d(v256 a, int imm) {
  v256 dst;
#include "xvslti_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvslti_d, 0);
  XFUZZ1(xvslti_d, 15);
}
