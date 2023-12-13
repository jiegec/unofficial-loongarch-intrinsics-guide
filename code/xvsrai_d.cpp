#include "common.h"

v256 xvsrai_d(v256 a, int imm) {
  v256 dst;
#include "xvsrai_d.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrai_d, 0);
  XFUZZ1(xvsrai_d, 7);
}
