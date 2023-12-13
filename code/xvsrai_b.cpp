#include "common.h"

v256 xvsrai_b(v256 a, int imm) {
  v256 dst;
#include "xvsrai_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrai_b, 0);
  XFUZZ1(xvsrai_b, 7);
}
