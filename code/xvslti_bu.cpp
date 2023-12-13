#include "common.h"

v256 xvslti_bu(v256 a, int imm) {
  v256 dst;
#include "xvslti_bu.h"
  return dst;
}

void test() {
  XFUZZ1(xvslti_bu, 0);
  XFUZZ1(xvslti_bu, 15);
}
