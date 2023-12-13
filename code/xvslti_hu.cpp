#include "common.h"

v256 xvslti_hu(v256 a, int imm) {
  v256 dst;
#include "xvslti_hu.h"
  return dst;
}

void test() {
  XFUZZ1(xvslti_hu, 0);
  XFUZZ1(xvslti_hu, 15);
}
