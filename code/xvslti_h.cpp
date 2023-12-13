#include "common.h"

v256 xvslti_h(v256 a, int imm) {
  v256 dst;
#include "xvslti_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvslti_h, 0);
  XFUZZ1(xvslti_h, 15);
}
