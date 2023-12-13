#include "common.h"

v256 xvsrlri_w(v256 a, int imm) {
  v256 dst;
#include "xvsrlri_w.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrlri_w, 0);
  XFUZZ1(xvsrlri_w, 7);
}
