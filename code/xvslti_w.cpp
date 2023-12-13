#include "common.h"

v256 xvslti_w(v256 a, int imm) {
  v256 dst;
#include "xvslti_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvslti_w, 0);
  XFUZZ1(xvslti_w, 15);
}
