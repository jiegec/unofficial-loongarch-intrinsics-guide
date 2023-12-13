#include "common.h"

v256 xvsrlri_d(v256 a, int imm) {
  v256 dst;
#include "xvsrlri_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrlri_d, 0);
  XFUZZ1(xvsrlri_d, 7);
}
