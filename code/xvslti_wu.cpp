#include "common.h"

v256 xvslti_wu(v256 a, int imm) {
  v256 dst;
#include "xvslti_wu.h"
  return dst;
}

void test() {
  XFUZZ1(xvslti_wu, 0);
  XFUZZ1(xvslti_wu, 15);
}
