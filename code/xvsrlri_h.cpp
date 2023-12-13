#include "common.h"

v256 xvsrlri_h(v256 a, int imm) {
  v256 dst;
#include "xvsrlri_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrlri_h, 0);
  XFUZZ1(xvsrlri_h, 7);
}
