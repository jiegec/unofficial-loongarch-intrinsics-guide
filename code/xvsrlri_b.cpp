#include "common.h"

v256 xvsrlri_b(v256 a, int imm) {
  v256 dst;
#include "xvsrlri_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrlri_b, 0);
  XFUZZ1(xvsrlri_b, 7);
}
