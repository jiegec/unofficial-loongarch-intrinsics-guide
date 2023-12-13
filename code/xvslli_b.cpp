#include "common.h"

v256 xvslli_b(v256 a, int imm) {
  v256 dst;
#include "xvslli_b.h"
  return dst;
}

void test() {
  XFUZZ1(xvslli_b, 0);
  XFUZZ1(xvslli_b, 7);
}
