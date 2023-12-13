#include "common.h"

v256 xvsrai_h(v256 a, int imm) {
  v256 dst;
#include "xvsrai_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvsrai_h, 0);
  XFUZZ1(xvsrai_h, 7);
}
