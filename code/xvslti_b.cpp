#include "common.h"

v256 xvslti_b(v256 a, int imm) {
  v256 dst;
#include "xvslti_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvslti_b, 0);
  XFUZZ1(xvslti_b, 15);
}
