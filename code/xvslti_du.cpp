#include "common.h"

v256 xvslti_du(v256 a, int imm) {
  v256 dst;
#include "xvslti_du.h"
  return dst;
}

void test() {
  XFUZZ1(xvslti_du, 0);
  XFUZZ1(xvslti_du, 15);
}
