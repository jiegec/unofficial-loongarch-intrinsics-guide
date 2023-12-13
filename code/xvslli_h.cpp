#include "common.h"

v256 xvslli_h(v256 a, int imm) {
  v256 dst;
#include "xvslli_h.h"
  return dst;
}

void test() {
  XFUZZ1(xvslli_h, 0);
  XFUZZ1(xvslli_h, 7);
}
